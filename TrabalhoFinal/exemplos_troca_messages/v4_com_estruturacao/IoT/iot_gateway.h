#ifndef iot_manager_h__
#define iot_manager_h__

#include <machine.h>
#include <alarm.h>
#include "messages/serialization.h"
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

    SmartObject * _currentObj;
    Service * _currentService;
    Parameter * _currentParameter;
public:
    IotGateway(const Protocol & p, NIC * nic)
    : _prot(p), _nic(nic), _currentObj(0), _currentService(0), _currentParameter(0) {
        _nic->attach(this, _prot);
    }

    //TODO botar estados!!
    void update(Observed * o, Protocol p, Buffer * b) {
        OStream cout;
        
        auto data = b->frame()->data<char>();

        cout << "Received " << b->size() << " bytes of payload from " << b->frame()->src() << " :";
        for(int i=0; i<b->size(); i++)
            cout << " " << hex << (unsigned char) data[i];
        cout << dec << endl;

        RegisterMessage * message = Serialization::deserialize(data);

        if(message->getType() == RegisterMessage::REGISTER_REQUEST){
            cout << "   Type: RegisterRequest" << endl;
            cout << "   Recebido do ID:";
            for(unsigned int i = 0; i<RegisterMessage::ID_SIZE; i++)
                cout << " " << hex << message->getId()[i];
            cout << dec << endl;

            //verifica no banco de dados
            cout << "   Preparando ACK..." << endl;

            RegisterResponse resp;
            resp.setId(message->getId());
            resp.setIsRegistered(false);
            auto msg = Serialization::serialize(&resp);
            _nic->send(b->frame()->src(), p, msg, resp.getSize()+2);
            delete msg;

            cout << "   ACK mandado!" << endl;
        }else if(message->getType() == RegisterMessage::REGISTER_OBJECT_REQUEST){
            cout << "   Type: RegisterObjectRequest" << endl;

            RegisterObjectRequest * request = reinterpret_cast<RegisterObjectRequest*>(message);
            _currentObj = request->getObject();

            cout << "   Name: " << _currentObj->getName() << endl;
            cout << "   Preparando ACK!" << endl;

            RegisterObjectResponse resp;
            resp.setId(message->getId());
            auto msg = Serialization::serialize(&resp);
            _nic->send(b->frame()->src(), p, msg, resp.getSize()+2);
            delete msg;

            cout << "   ACK mandado!" << endl;
        }else if(message->getType() == RegisterMessage::REGISTER_SERVICE_REQUEST){
            cout << "   Type: RegisterServiceRequest" << endl;

            RegisterServiceRequest * request = reinterpret_cast<RegisterServiceRequest*>(message);
            _currentService = request->getService();
            _currentObj->addService(_currentService);

            cout << "   Name: " << _currentService->getName() << endl;
            cout << "   Preparando ACK!" << endl;

            RegisterServiceResponse resp;
            resp.setId(message->getId());
            auto msg = Serialization::serialize(&resp);
            _nic->send(b->frame()->src(), p, msg, resp.getSize()+2);
            delete msg;

            cout << "   ACK mandado!" << endl;
        }else if(message->getType() == RegisterMessage::REGISTER_PARAMETER_REQUEST){
            cout << "   Type: RegisterParameterRequest" << endl;

            RegisterParameterRequest * request = 
                reinterpret_cast<RegisterParameterRequest*>(message);
            _currentParameter = request->getParameter();
            _currentService->addParameter(_currentParameter);

            cout << "   Name: " << _currentParameter->getName() << endl;
            cout << "   Preparando ACK!" << endl;

            RegisterParameterResponse resp;
            resp.setId(message->getId());
            auto msg = Serialization::serialize(&resp);
            _nic->send(b->frame()->src(), p, msg, resp.getSize()+2);
            delete msg;

            cout << "   ACK mandado!" << endl;
        }else if(message->getType() == RegisterMessage::REGISTER_END_OBJECT_REQUEST){
            cout << "   Type: RegisterEndObjectRequest" << endl;

            // Manda pro banco de dados...
            cout << "   Preparando ACK..." << endl;

            RegisterEndObjectResponse resp;
            resp.setId(message->getId());
            auto msg = Serialization::serialize(&resp);
            _nic->send(b->frame()->src(), p, msg, resp.getSize()+2);
            delete msg;

            cout << "   ACK mandado!" << endl;
            cout << "Encerrada a fase de cadastro do object: "
                << _currentObj->getName() << ". Esperando pelo proximo cadastro..." << endl;
            
            delete _currentObj; _currentObj = 0;
            delete _currentService; _currentService = 0;
            delete _currentParameter; _currentParameter = 0;
        }
        cout << endl;

        _nic->free(b);
        if(message != 0)
            delete message;
    }
};

};

#endif