#ifndef iot_manager_h__
#define iot_manager_h__

#include <machine.h>
#include "objects/smartobject.h"
#include "messages/register_serialization.h"
#include "messages/register_message.h"

#include "objects/parametertype.h"
#include "objects/parameter_combo.h"

using namespace EPOS;

namespace IoT {
    
class IotManager : public IEEE802_15_4::Observer {
public:
    typedef NIC::Address Address;
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;
protected:
    SmartObject * _object;
    NIC *_nic;
    OStream cout;

    Service_List_Element * _currentService;
    Parameter_List_Element * _currentParameter;
    Option_List_Element * _currentOption;
public:
    IotManager(const Protocol & p, NIC * nic, SmartObject * object)
    : _object(object), _nic(nic), 
      _currentService(0), _currentParameter(0), _currentOption(0) {
        _nic->attach(this, p);
    }

    void run(){
        cout << "# Mandando RegisterRequest via broadcast!" << endl;
        RegisterRequest request;
        sendRequest(&request, _nic->broadcast(), IEEE802_15_4::ELP);
    }

    void update(Observed * o, Protocol p, Buffer * b) {
        auto data = b->frame()->data<char>();

        cout << "# Received " << b->size() << " bytes of payload from " << b->frame()->src() << " :";
        for(int i=0; i<b->size(); i++)
            cout << " " << (unsigned char) data[i];
        cout << endl;

        RegisterMessage * message = RegisterSerialization::deserialize(data);
        if(message->getId() == _object->getId()){
            if(message->getType() == RegisterMessage::REGISTER_RESPONSE){
                cout << "#   Type: RegisterResponse!" << endl;
                RegisterResponse *resp = reinterpret_cast<RegisterResponse*>(message);
                cout << "#   isRegistered: " << resp->isRegistered() << endl;

                if(!resp->isRegistered()){
                    cout << "# Mandando object: " << 
                        _object->getName() << "..." << endl;

                    RegisterObjectRequest request(_object);
                    sendRequest(&request, b->frame()->src(), p);
                }else{
                    cout << "# Object ja cadastrado, aguardando por comandos..." << endl << endl;
                }
            }else if(message->getType() == RegisterMessage::REGISTER_OBJECT_RESPONSE){
                cout << "#   Type: RegisterObjectResponse!" << endl;
                
                _currentService = _object->getServices()->head();
                sendService(b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_SERVICE_RESPONSE){
                cout << "#   Type: RegisterServiceResponse!" << endl;
                
                _currentParameter = _currentService->object()->getParameters()->head();
                sendParameter(b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_PARAMETER_RESPONSE){
                cout << "#   Type: RegisterParameterResponse!" << endl;

                // Verifica se não eh um combo...
                if(_currentParameter->object()->getType() == ParameterType::COMBO){
                    ParameterType * type = _currentParameter->object()->getParameterType();
                    ParameterCombo * combo = reinterpret_cast<ParameterCombo*>(type);

                    _currentOption = combo->getOptionsList()->head();
                    sendOption(b->frame()->src(), p);
                }else{
                    _currentParameter = _currentParameter->next();
                    sendParameter(b->frame()->src(), p);
                }
            }else if(message->getType() == RegisterMessage::REGISTER_OPTION_RESPONSE){
                cout << "#   Type: RegisterOptionResponse!" << endl;
                
                _currentOption = _currentOption->next();
                sendOption(b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_END_OBJECT_RESPONSE){
                cout << "#   Type: RegisterEndObjectResponse!" << endl;
                cout << "# Fase de cadastro terminada! Aguardando comandos..." << endl;
            }
        }
        cout << endl;

        _nic->free(b);
        if(message != 0)
            delete message;
    }
protected:
    void sendRequest(RegisterMessage * request, const Address & dst, const Protocol & prot){
        auto msg = RegisterSerialization::serialize(request);
        _nic->send(dst, prot, msg, request->getSize()+2);
        delete msg;
    }

    void sendService(const Address & dst, const Protocol & p){
        if(_currentService){
            cout << "# Mandando servico: " << 
                _currentService->object()->getName() << "..." << endl;                        
            
            RegisterServiceRequest request(_currentService->object());
            sendRequest(&request, dst, p);
        }else{
            cout << "# Fim dos servicos do object: " 
                << _object->getName() << "..." << endl;
            cout << "# Encerrando cadastro..." << endl;

            RegisterEndObjectRequest request;
            sendRequest(&request, dst, p);
        }
    }

    void sendParameter(const Address & dst, const Protocol & p){
        if(_currentParameter){
            cout << "# Mandando parametro: " << 
                _currentParameter->object()->getName() << "..." << endl;
            
            RegisterParameterRequest request(_currentParameter->object());
            sendRequest(&request, dst, p);
        }else{
            cout << "# Fim dos parametros do servico: " 
                << _currentService->object()->getName() << "..." << endl;

            _currentService = _currentService->next();
            sendService(dst, p);
        }
    }

    void sendOption(const Address & dst, const Protocol & p){
        if(_currentOption){
            cout << "# Mandando option: " 
                << _currentOption->object() << "..." << endl;
                
            RegisterOptionRequest request(_currentOption->object());
            sendRequest(&request, dst, p);
        }else{
            cout << "# Fim das options do parametro: "
                << _currentParameter->object()->getName() << "..." << endl;

            _currentParameter = _currentParameter->next();
            sendParameter(dst, p);
        }
    }
};

};

#endif