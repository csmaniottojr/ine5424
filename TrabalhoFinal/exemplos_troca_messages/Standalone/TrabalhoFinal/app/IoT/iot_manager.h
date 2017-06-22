#ifndef iot_manager_h__
#define iot_manager_h__

#include <machine.h>
#include "objects/smartobject.h"
#include "messages/serialization_register.h"
#include "messages/register_message.h"

#include "objects/parametertype.h"
#include "objects/parameter_combo.h"

using namespace EPOS;

namespace IoT {

    class Iot_Manager : public IEEE802_15_4::Observer
    {
    public:
        typedef NIC::Address Address;
        typedef IEEE802_15_4::Protocol Protocol;
        typedef IEEE802_15_4::Buffer Buffer;
        typedef IEEE802_15_4::Frame Frame;
        typedef IEEE802_15_4::Observed Observed;
    protected:
        Smart_Object * _object;
        NIC *_nic;
        OStream cout;

        Service_List_Element * _currentService;
        Parameter_List_Element * _currentParameter;
        Option_List_Element * _currentOption;
    public:

        Iot_Manager ( const Protocol & p, NIC * nic, Smart_Object * object )
        : _object ( object ), _nic ( nic ),
        _currentService ( 0 ), _currentParameter ( 0 ), _currentOption ( 0 ) {
            _nic->attach ( this, p );
        }

        void run ( ) {
            cout << "Mandando RegisterRequest via broadcast!" << endl;
            RegisterRequest request;
            send_request ( &request, _nic->broadcast ( ), IEEE802_15_4::ELP );
        }

        void update ( Observed * o, Protocol p, Buffer * b ) {
            auto data = b->frame ( )->data<char>( );

            cout << "Received " << b->size ( ) << " bytes of payload from " << b->frame ( )->src ( ) << " :";
            for ( int i = 0; i < b->size ( ); i++ )
                cout << " " << hex << ( unsigned char ) data[i];
            cout << dec << endl;

            RegisterMessage * message = SerializationRegister::deserialize ( data );
            if ( memcmp ( message->getId ( ), _object->getId ( ), RegisterMessage::ID_SIZE ) == 0 ) {
                if ( message->getType ( ) == RegisterMessage::REGISTER_RESPONSE ) {
                    cout << "   Type: RegisterResponse!" << endl;
                    RegisterResponse *resp = reinterpret_cast < RegisterResponse* > ( message );
                    cout << "   isRegistered: " << resp->isRegistered ( ) << endl;

                    if ( !resp->isRegistered ( ) ) {
                        cout << "Mandando object: " <<
                                _object->name ( ) << "..." << endl;

                        RegisterObjectRequest request ( _object );
                        send_request ( &request, b->frame ( )->src ( ), p );
                    } else {
                        cout << "Object ja cadastrado, aguardando por comandos..." << endl << endl;
                    }
                } else if ( message->getType ( ) == RegisterMessage::REGISTER_OBJECT_RESPONSE ) {
                    cout << "   Type: RegisterObjectResponse!" << endl;

                    _currentService = _object->getServices ( )->head ( );
                    send_service ( b->frame ( )->src ( ), p );
                } else if ( message->getType ( ) == RegisterMessage::REGISTER_SERVICE_RESPONSE ) {
                    cout << "   Type: RegisterServiceResponse!" << endl;

                    _currentParameter = _currentService->object ( )->getParameters ( )->head ( );
                    send_parameter ( b->frame ( )->src ( ), p );
                } else if ( message->getType ( ) == RegisterMessage::REGISTER_PARAMETER_RESPONSE ) {
                    cout << "   Type: RegisterParameterResponse!" << endl;

                    // Verifica se não eh um combo...
                    if ( _currentParameter->object ( )->getType ( ) == Parameter_Type::COMBO ) {
                        Parameter_Type * type = _currentParameter->object ( )->getParameterType ( );
                        Parameter_Combo * combo = reinterpret_cast < Parameter_Combo* > ( type );

                        _currentOption = combo->get_options_list ( )->head ( );
                        send_option ( b->frame ( )->src ( ), p );
                    } else {
                        _currentParameter = _currentParameter->next ( );
                        send_parameter ( b->frame ( )->src ( ), p );
                    }
                } else if ( message->getType ( ) == RegisterMessage::REGISTER_OPTION_RESPONSE ) {
                    cout << "   Type: RegisterOptionResponse!" << endl;

                    _currentOption = _currentOption->next ( );
                    send_option ( b->frame ( )->src ( ), p );
                } else if ( message->getType ( ) == RegisterMessage::REGISTER_END_OBJECT_RESPONSE ) {
                    cout << "   Type: RegisterEndObjectResponse!" << endl;
                    cout << "Fase de cadastro terminada! Aguardando comandos..." << endl;
                }
            }
            cout << endl;

            _nic->free ( b );
            if ( message != 0 )
                delete message;
        }
    protected:

        void send_request ( RegisterMessage * request, const Address & dst, const Protocol & prot ) {
            auto msg = SerializationRegister::serialize ( request );
            _nic->send ( dst, prot, msg, request->getSize ( ) + 2 );
            delete msg;
        }

        void send_service ( const Address & dst, const Protocol & p ) {
            if ( _currentService ) {
                cout << "Mandando servico: " <<
                        _currentService->object ( )->getName ( ) << "..." << endl;

                RegisterServiceRequest request ( _currentService->object ( ) );
                send_request ( &request, dst, p );
            } else {
                cout << "Fim dos servicos do object: "
                        << _object->name ( ) << "..." << endl;
                cout << "Encerrando cadastro..." << endl;

                RegisterEndObjectRequest request;
                send_request ( &request, dst, p );
            }
        }

        void send_parameter ( const Address & dst, const Protocol & p ) {
            if ( _currentParameter ) {
                cout << "Mandando parametro: " <<
                        _currentParameter->object ( )->getName ( ) << "..." << endl;

                RegisterParameterRequest request ( _currentParameter->object ( ) );
                send_request ( &request, dst, p );
            } else {
                cout << "Fim dos parametros do servico: "
                        << _currentService->object ( )->getName ( ) << "..." << endl;

                _currentService = _currentService->next ( );
                send_service ( dst, p );
            }
        }

        void send_option ( const Address & dst, const Protocol & p ) {
            if ( _currentOption ) {
                cout << "Mandando option: "
                        << _currentOption->object ( ) << "..." << endl;

                RegisterOptionRequest request ( _currentOption->object ( ) );
                send_request ( &request, dst, p );
            } else {
                cout << "Fim das options do parametro: "
                        << _currentParameter->object ( )->getName ( ) << "..." << endl;

                _currentParameter = _currentParameter->next ( );
                send_parameter ( dst, p );
            }
        }
    } ;

};

#endif