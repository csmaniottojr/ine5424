#ifndef iot_gateway_h__
#define iot_gateway_h__

#include <machine.h>
#include <usb.h>

#include "../cheats/led.h"
#include "messages/register_serialization.h"
#include "messages/command_serialization.h"
#include "messages/register_message.h"
#include "messages/command_message.h"

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
public:
    IotGateway(const Protocol & p, NIC * nic)
    : _prot(p), _nic(nic) {
        _nic->attach(this, _prot);
    }
    
    void update(Observed * o, Protocol p, Buffer * b) {
        auto data = b->frame()->data<char>();

        //eMoteIII::led::blink(0.1, 1);
        cout << "# Received " << b->size() << " bytes of payload from " << b->frame()->src() << endl;
        /*for(int i=0; i<b->size(); i++)
            cout << " " << (unsigned char) data[i];
        cout << endl;*/

        switch(data[0]){
            case RegisterMessage::START_CHAR: {
                RegisterMessage * message = RegisterSerialization::simpleDeserialize(data);
                //Verifica se eh uma mensagem valida
                if(message->getType() >= RegisterMessage::FIRST_TYPE &&
                    message->getType() <= RegisterMessage::LAST_TYPE){
                    //Envia a mensagem pra ser tratada no PC
                    USB::put(data, message->getSize());
                }
                delete message;
                break;
            }case CommandMessage::START_CHAR: {
                CommandMessage * message = CommandSerialization::simpleDeserialize(data);
                //Verifica se eh uma mensagem valida
                if(message->getType() >= CommandMessage::FIRST_TYPE &&
                    message->getType() <= CommandMessage::LAST_TYPE){
                    //Envia a mensagem pra ser tratada no PC
                    USB::put(data, message->getSize());
                }
                delete message;
                break;
            }default: break;
        }

        //cout << endl;
        _nic->free(b);
    }

    void receive(const char * msg, int length){
        eMoteIII::led::blink(0.1, 1);
        /*cout << "# Mensagem recebida do USB:"; 
        for(int i=0; i<length; i++)
            cout << " " << (unsigned char) msg[i];
        cout << endl;*/

        switch(msg[0]){
            case RegisterMessage::START_CHAR:{
                RegisterMessage * message = RegisterSerialization::simpleDeserialize(msg);
                //Verifica se eh uma mensagem valida
                if(message->getType() >= RegisterMessage::FIRST_TYPE &&
                    message->getType() <= RegisterMessage::LAST_TYPE){
                    //Envia a mensagem por broadcast para ser interpretada pelo object
                    _nic->send(_nic->broadcast(), _prot, msg, message->getSize()+2);
                }
                delete message;
                break;
            }case CommandMessage::START_CHAR:{
                CommandMessage * message = CommandSerialization::simpleDeserialize(msg);
                //Verifica se eh uma mensagem valida
                if(message->getType() >= CommandMessage::FIRST_TYPE &&
                    message->getType() <= CommandMessage::LAST_TYPE){
                    //Envia a mensagem por broadcast para ser interpretada pelo object
                    _nic->send(_nic->broadcast(), _prot, msg, message->getSize()+2);
                }
                delete message;
                break;
            }default: break;
        }
    }
};

};

#endif