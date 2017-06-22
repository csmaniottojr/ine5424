#ifndef iot_gateway_h__
#define iot_gateway_h__

#include <machine.h>
#include <alarm.h>

#include "../cheats/led.h"
#include "messages/serialization_register.h"
#include "messages/register_message.h"
#include "objects/smartobject.h"

using namespace EPOS;

namespace IoT {
    
class IotGateway : public IEEE802_15_4::Observer {
public:
    typedef NIC::Address Address;
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;
protected:
    Protocol _prot;
    NIC *_nic;
    OStream cout;

    SmartObject * _currentObj;
    Service * _currentService;
    Parameter * _currentParameter;
public:
    IotGateway(const Protocol & p, NIC * nic)
    : _prot(p), _nic(nic), _currentObj(0), _currentService(0), _currentParameter(0) {
        _nic->attach(this, _prot);
    }
    
    void update(Observed * o, Protocol p, Buffer * b) {
        auto data = b->frame()->data<char>();

        eMoteIII::led::blink(0.1, 1);

        cout << "Received " << b->size() << " bytes of payload from " << b->frame()->src() << " :";
        for(int i=0; i<b->size(); i++)
            cout << " " << hex << (unsigned char) data[i];
        cout << dec << endl;

        RegisterMessage * message = SerializationRegister::deserialize(data);

        if(_currentObj == 0 || message->getId() == _currentObj->getId()){
            if(message->getType() == RegisterMessage::REGISTER_REQUEST){
                cout << "   Type: RegisterRequest" << endl;
                cout << "   Recebido do ID: " << message->getId() << endl;

                //TODO verificar no banco de dados

                RegisterResponse resp;
                resp.setId(message->getId());
                resp.setIsRegistered(false);
                sendRequest(&resp, b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_OBJECT_REQUEST){
                cout << "   Type: RegisterObjectRequest" << endl;

                RegisterObjectRequest * request = reinterpret_cast<RegisterObjectRequest*>(message);
                _currentObj = request->getObject();

                cout << "   Name: " << _currentObj->getName() << endl;

                RegisterObjectResponse resp;
                resp.setId(message->getId());
                sendRequest(&resp, b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_SERVICE_REQUEST){
                cout << "   Type: RegisterServiceRequest" << endl;

                RegisterServiceRequest * request = reinterpret_cast<RegisterServiceRequest*>(message);
                _currentService = request->getService();
                _currentObj->addService(_currentService);

                cout << "   Name: " << _currentService->getName() << endl;

                RegisterServiceResponse resp;
                resp.setId(message->getId());
                sendRequest(&resp, b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_PARAMETER_REQUEST){
                cout << "   Type: RegisterParameterRequest" << endl;

                RegisterParameterRequest * request = 
                    reinterpret_cast<RegisterParameterRequest*>(message);
                _currentParameter = request->getParameter();
                _currentService->addParameter(_currentParameter);

                cout << "   Name: " << _currentParameter->getName() << endl;

                RegisterParameterResponse resp;
                resp.setId(message->getId());
                sendRequest(&resp, b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_OPTION_REQUEST){
                cout << "   Type: RegisterOptionRequest" << endl;

                RegisterOptionRequest * request = 
                    reinterpret_cast<RegisterOptionRequest*>(message);
                
                ParameterType * type = _currentParameter->getParameterType();
                ParameterCombo * combo = reinterpret_cast<ParameterCombo*>(type);
                combo->addOption(request->getOption());

                cout << "   Option: " << request->getOption() << endl;

                RegisterOptionResponse resp;
                resp.setId(message->getId());
                sendRequest(&resp, b->frame()->src(), p);
            }else if(message->getType() == RegisterMessage::REGISTER_END_OBJECT_REQUEST){
                cout << "   Type: RegisterEndObjectRequest" << endl;

                //TODO Mandar pro banco de dados...

                RegisterEndObjectResponse resp;
                resp.setId(message->getId());
                sendRequest(&resp, b->frame()->src(), p);

                cout << "Encerrada a fase de cadastro do object: "
                    << _currentObj->getName() << ". Esperando pelo proximo cadastro..." << endl;
                
                delete _currentObj; _currentObj = 0;
                delete _currentService; _currentService = 0;
                delete _currentParameter; _currentParameter = 0;
            }
        }
        cout << endl;

        _nic->free(b);
        if(message != 0)
            delete message;
    }

    void receive(const char * msg, int lenght){
        cout << "Mensagem recebida do USB: " << msg << endl;
    }   

protected:
    void sendRequest(RegisterMessage * request, const Address & dst, const Protocol & prot){
        auto msg = SerializationRegister::serialize(request);
        _nic->send(dst, prot, msg, request->getSize()+2);
        delete msg;
        cout << "   ACK mandado!" << endl;
    }
};

};

#endif