#ifndef iot_manager_h__
#define iot_manager_h__

#include <machine.h>
#include "objects/smartobject.h"
#include "messages/register_serialization.h"
#include "messages/command_serialization.h"
#include "messages/register_message.h"
#include "messages/command_message.h"

#include "objects/parametertype.h"
#include "objects/parameter_combo.h"

using namespace EPOS;

namespace IoT {

    class IotManager : public IEEE802_15_4::Observer
    {
    public:
        typedef SmartObject::ID ID;
        typedef NIC::Address Address;
        typedef IEEE802_15_4::Protocol Protocol;
        typedef IEEE802_15_4::Buffer Buffer;
        typedef IEEE802_15_4::Frame Frame;
        typedef IEEE802_15_4::Observed Observed;
    protected:
        SmartObject * _object;
        NIC *_nic;
        OStream cout;

        bool _registered;

        Service_List_Element * _currentService;
        Parameter_List_Element * _currentParameter;
        Option_List_Element * _currentOption;
    public:

        IotManager ( const Protocol & p, NIC * nic, SmartObject * object )
        : _object ( object ), _nic ( nic ), _registered ( false ),
        _currentService ( 0 ), _currentParameter ( 0 ), _currentOption ( 0 ) {
            assert ( object != 0 );
            _nic->attach ( this, p );
        }

        void run ( ) {
            if(!_registered){
                cout << "# Mandando RegisterRequest via broadcast!" << endl;
                RegisterRequest request;
                sendRegisterMessage ( &request, _nic->broadcast ( ), IEEE802_15_4::ELP );
            }
        }

        void update ( Observed * o, Protocol p, Buffer * b ) {
            auto data = b->frame ( )->data<char>( );

            // Verifica se a mensagem é para mim...
            ID id = *( ( ID* ) & data[2] );
            if ( id != _object->getId ( ) ) {
                cout << "# Messagem ignorada..." << endl << endl;
                _nic->free ( b );
                return;
            }

            cout << "# Received " << b->size ( ) << " bytes of payload from " << b->frame ( )->src ( ) << " :";
            for ( int i = 0; i < b->size ( ); i++ )
                cout << " " << ( unsigned char ) data[i];
            cout << endl;

            switch ( data[0] ) {
                case RegisterMessage::START_CHAR:
                {
                    if ( !_registered )
                        processRegisterMessage ( data, b->frame ( )->src ( ), p );
                    break;
                }
                case CommandMessage::START_CHAR:
                {
                    if ( _registered )
                        processCommandMessage ( data, b->frame ( )->src ( ), p );
                    break;
                }
                default: break;
            }

            cout << endl;
            _nic->free ( b );
        }
    protected:

        void processRegisterMessage ( const char * msg, const Address & dst, const Protocol & prot ) {
            RegisterMessage * message = RegisterSerialization::deserialize ( msg );
            if ( message->getType ( ) == RegisterMessage::REGISTER_RESPONSE ) {
                cout << "#   Type: RegisterResponse!" << endl;
                RegisterResponse *resp = reinterpret_cast < RegisterResponse* > ( message );
                cout << "#   isRegistered: " << resp->isRegistered ( ) << endl;

                if ( !resp->isRegistered ( ) ) {
                    cout << "# Mandando object: " <<
                            _object->getName ( ) << "..." << endl;

                    RegisterObjectRequest request ( _object );
                    sendRegisterMessage ( &request, dst, prot );
                } else {
                    cout << "# Object ja cadastrado, aguardando por comandos..." << endl << endl;
                    _registered = true;
                }
            } else if ( message->getType ( ) == RegisterMessage::REGISTER_OBJECT_RESPONSE ) {
                cout << "#   Type: RegisterObjectResponse!" << endl;

                _currentService = _object->getServices ( )->head ( );
                sendService ( dst, prot );
            } else if ( message->getType ( ) == RegisterMessage::REGISTER_SERVICE_RESPONSE ) {
                cout << "#   Type: RegisterServiceResponse!" << endl;

                _currentParameter = _currentService->object ( )->getParameters ( )->head ( );
                sendParameter ( dst, prot );
            } else if ( message->getType ( ) == RegisterMessage::REGISTER_PARAMETER_RESPONSE ) {
                cout << "#   Type: RegisterParameterResponse!" << endl;

                // Verifica se não eh um combo...
                if ( _currentParameter->object ( )->getType ( ) == ParameterType::COMBO ) {
                    ParameterType * type = _currentParameter->object ( )->getParameterType ( );
                    ParameterCombo * combo = reinterpret_cast < ParameterCombo* > ( type );

                    _currentOption = combo->getOptionsList ( )->head ( );
                    sendOption ( dst, prot );
                } else {
                    _currentParameter = _currentParameter->next ( );
                    sendParameter ( dst, prot );
                }
            } else if ( message->getType ( ) == RegisterMessage::REGISTER_OPTION_RESPONSE ) {
                cout << "#   Type: RegisterOptionResponse!" << endl;

                _currentOption = _currentOption->next ( );
                sendOption ( dst, prot );
            } else if ( message->getType ( ) == RegisterMessage::REGISTER_END_OBJECT_RESPONSE ) {
                cout << "#   Type: RegisterEndObjectResponse!" << endl;
                cout << "# Fase de cadastro terminada! Aguardando comandos..." << endl;
                _registered = true;
            }

            delete message;
        }

        void processCommandMessage ( const char * msg, const Address & dst, const Protocol & prot ) {
            //Mensagens de comando
            CommandMessage * message = CommandSerialization::deserialize ( msg );
            CommandMessage * response = new CommandMessage();
            response->setRegisterId(message->getRegisterId());
            unsigned char * resp_data = 0;

            if ( message->getType ( ) == CommandMessage::COMMAND_READ_REQUEST ) {
                cout << "#   Type: CommandReadRequest!" << endl;
                response->setType(CommandMessage::COMMAND_READ_RESPONSE);

                auto id = message->getRegisterId ( );
                auto parameter = _object->getById ( id );
                if ( parameter == 0 ) {
                    cout << "#  [WARNING!]CAN'T FIND A PARAMETER WITH ID " << id << ", COMMAND DUMPED!\n";
                    // Manda resposta sem dado do mesmo jeito!?
                }else{
                    switch ( parameter->getType ( ) ) {
                        case ParameterType::BOOLEAN:
                        {
                            resp_data = new unsigned char[1];
                            bool data = parameter->boolValue ( );
                            ((bool*) resp_data)[0] = data;
                            response->setData(resp_data, 1);
                            break;
                        }
                        case ParameterType::FLOAT:
                        {
                            resp_data = new unsigned char[4];
                            float data = parameter->floatValue ( );
                            ((float*) resp_data)[0] = data;
                            response->setData(resp_data, 4);
                            break;
                        }
                        case ParameterType::COMBO:
                        {
                            resp_data = new unsigned char[4];
                            int index = parameter->comboValue ( );
                            ((int*) resp_data)[0] = index;
                            response->setData(resp_data, 4);
                            break;
                        }
                        default:
                        {
                            cout << "#  [WARNING!] Unknown parameter type requested for read!\n";
                            // Manda resposta sem dado do mesmo jeito!?
                            break;
                        }
                    }
                }
            } else if ( message->getType ( ) == CommandMessage::COMMAND_WRITE_REQUEST ) {
                cout << "#   Type: CommandWriteRequest!" << endl;
                response->setType(CommandMessage::COMMAND_WRITE_RESPONSE);

                auto id = message->getRegisterId ( );
                auto parameter = _object->getById ( id );
                if ( parameter == 0 ) {
                    cout << "#  [WARNING!]CAN'T FIND A PARAMETER WITH ID " << id << ", COMMAND DUMPED!\n";
                    // Manda resposta sem dado do mesmo jeito!?
                }else{
                    switch ( parameter->getType ( ) ) {
                        case ParameterType::BOOLEAN:
                        {
                            bool data = *(( bool* )message->getData ( ));
                            parameter->update ( data );
                            break;
                        }
                        case ParameterType::FLOAT:
                        {
                            float data = *(( float* ) message->getData ( ));
                            parameter->update ( data );
                            break;
                        }
                        case ParameterType::COMBO:
                        {
                            int index = *(( int* ) message->getData ( ));
                            parameter->update ( index );
                            break;
                        }
                        default:
                        {
                            cout << "#  [WARNING!] Unknown parameter type requested for read!\n";
                            // Manda resposta sem dado do mesmo jeito!?
                            break;
                        }
                    }
                }
            }
            sendCommandMessage(response, dst, prot);
            delete[] resp_data;
            delete response;
            delete message;
        }

    protected:

        void sendRegisterMessage ( RegisterMessage * message, const Address & dst, const Protocol & prot ) {
            auto msg = RegisterSerialization::serialize ( message );
            _nic->send ( dst, prot, msg, message->getSize ( ) + 2 );
            delete[] msg;
        }

        void sendCommandMessage ( CommandMessage * message, const Address & dst, const Protocol & prot ) {
            auto msg = CommandSerialization::serialize ( message );
            _nic->send ( dst, prot, msg, message->getSize ( ) + 2 );
            delete[] msg;
        }

        void sendService ( const Address & dst, const Protocol & p ) {
            if ( _currentService ) {
                cout << "# Mandando servico: " <<
                        _currentService->object ( )->getName ( ) << "..." << endl;

                RegisterServiceRequest request ( _currentService->object ( ) );
                sendRegisterMessage ( &request, dst, p );
            } else {
                cout << "# Fim dos servicos do object: "
                        << _object->getName ( ) << "..." << endl;
                cout << "# Encerrando cadastro..." << endl;

                RegisterEndObjectRequest request;
                sendRegisterMessage ( &request, dst, p );
            }
        }

        void sendParameter ( const Address & dst, const Protocol & p ) {
            if ( _currentParameter ) {
                cout << "# Mandando parametro: " <<
                        _currentParameter->object ( )->getName ( ) << "..." << endl;

                RegisterParameterRequest request ( _currentParameter->object ( ) );
                sendRegisterMessage ( &request, dst, p );
            } else {
                cout << "# Fim dos parametros do servico: "
                        << _currentService->object ( )->getName ( ) << "..." << endl;

                _currentService = _currentService->next ( );
                sendService ( dst, p );
            }
        }

        void sendOption ( const Address & dst, const Protocol & p ) {
            if ( _currentOption ) {
                cout << "# Mandando option: "
                        << _currentOption->object ( ) << "..." << endl;

                RegisterOptionRequest request ( _currentOption->object ( ) );
                sendRegisterMessage ( &request, dst, p );
            } else {
                cout << "# Fim das options do parametro: "
                        << _currentParameter->object ( )->getName ( ) << "..." << endl;

                _currentParameter = _currentParameter->next ( );
                sendParameter ( dst, p );
            }
        }
    } ;

};

#endif