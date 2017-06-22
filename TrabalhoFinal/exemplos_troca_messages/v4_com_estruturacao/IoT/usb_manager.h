#ifndef usb_manager_h__
#define usb_manager_h__

#include <ieee802_15_4.h>
#include <usb.h>
#include "iot_gateway.h"
#include "../cheats/led.h"

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
            int len;
			bool ok_message = false;
            memset(_msg, '\0', MAX_LENGHT);
			while(!ok_message) {
				ok_message = true;
				len = 0;
				_msg[len++] = USB::get();
				_msg[len++] = USB::get();
                while(!((_msg[len-2] == '\r') 
                    && (_msg[len-1] == '\n'))) {
					_msg[len++] = USB::get();
					if(len > MAX_LENGHT) {
						ok_message = false;
						break;
					}
				}
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
};

};

#endif