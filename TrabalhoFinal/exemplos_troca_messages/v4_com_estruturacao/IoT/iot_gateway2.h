#ifndef iot_gateway_h__
#define iot_gateway_h__

#include <machine.h>
#include <alarm.h>

#include "../cheats/led.h"
#include "messages/register_serialization.h"
#include "messages/register_message.h"
#include "objects/smartobject.h"

using namespace EPOS;

namespace IoT {
    
class IotGateway2 : public IEEE802_15_4::Observer {
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
public:
    IotGateway2(const Protocol & p, NIC * nic)
    : _prot(p), _nic(nic) {
        _nic->attach(this, _prot);
    }
    
    void update(Observed * o, Protocol p, Buffer * b) {
        auto data = b->frame()->data<char>();

        eMoteIII::led::blink(0.1, 1);

        switch(data[0]){
            case ';': {//RegisterMessage
                RegisterMessage * message = RegisterSerialization::simpleDeserialize(msg);
                //Verifica se eh uma mensagem valida
                if(message->getType() >= RegisterMessage::REGISTER_REQUEST &&
                    message->getType() <= RegisterMessage::REGISTER_END_OBJECT_RESPONSE){
                    //Envia a mensagem pra ser tratada no PC
                    USBManager::send(data, message->getSize());
                }
                delete message;
                break;
            }case ':': {//CommandMessage

                break;
            }default: break;
        }

        _nic->free(b);
    }

    void receive(const char * msg, int lenght){
        cout << "Mensagem recebida do USB: " << msg << endl;

        switch(msg[0]){
            case ';':{//RegisterMessage
                RegisterMessage * message = RegisterSerialization::simpleDeserialize(msg);
                //Verifica se eh uma mensagem valida
                if(message->getType() >= RegisterMessage::REGISTER_REQUEST &&
                    message->getType() <= RegisterMessage::REGISTER_END_OBJECT_RESPONSE){
                    //Envia a mensagem por broadcast para ser interpretada pelo object
                    _nic->send(_nic->broadcast(), prot, msg, message->getSize()+2);
                }
                delete message;
                break;
            }case ':':{//CommandMessage

                break;
            }default: break;
        }
    }
};

};

#endif