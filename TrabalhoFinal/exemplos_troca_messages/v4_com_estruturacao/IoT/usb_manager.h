#ifndef usb_manager_h__
#define usb_manager_h__

#include <ieee802_15_4.h>
#include <usb.h>
#include "iot_gateway.h"
#include "../cheats/led.h"

#include "messages/command_message.h"
#include "messages/register_message.h"

using namespace EPOS;

namespace IoT {

class USBManager {
protected:
    static const unsigned int MAX_LENGHT = IEEE802_15_4::Frame::MTU;
public:
    static int run(IotGateway * _gateway){
        OStream cout;
        char _msg[MAX_LENGHT];

        while(true){
            int len = 0;
            memset(_msg, '\0', MAX_LENGHT);
            _msg[len++] = USB::get();
            _msg[len++] = USB::get();
            if(!isValidMsg(_msg[0], _msg[1]))
                continue;
            while(len < ((unsigned char)_msg[1])) {
                _msg[len++] = USB::get();
            }
            if(_gateway != 0){
                char * msg = new char[len+1];
                memset(msg, '\0', len+1);
                memcpy(msg, _msg, len);
                _gateway->receive(msg, len+1);
                delete msg;
            }
        }
    }

    static void send(const char * msg, unsigned int size){
        USB::put(msg, size);
    }
    
protected:
    static bool isValidMsg(char start_char, unsigned char size){
        return ((start_char == RegisterMessage::START_CHAR && 
            size >= RegisterMessage::BASE_SIZE) ||
                (start_char == CommandMessage::START_CHAR && 
            size >= CommandMessage::BASE_SIZE)) &&
            size <= MAX_LENGHT;
    }
};

};

#endif