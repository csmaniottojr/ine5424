#ifndef usb_manager_h__
#define usb_manager_h__

#include <modbus_ascii.h>
#include <usb.h>
#include "iot_gateway.h"
#include "../cheats/led.h"

using namespace EPOS;

namespace IoT {

class USBManager {
protected:
public:
    static int run(IotGateway * _gateway){
        OStream cout;
        USB usb;

        char _msg[Modbus_ASCII::MSG_LEN];
        memset(_msg, '\0', Modbus_ASCII::MSG_LEN);
        while(true){
            int len;
			bool ok_message = false;
            memset(_msg, '\0', Modbus_ASCII::MSG_LEN);
			while(!ok_message) {
				ok_message = true;
				len = 0;
				_msg[len++] = usb.get();
				_msg[len++] = usb.get();
				while(!((_msg[len-2] == '\r') 
                    && (_msg[len-1] == '\n'))) {
					_msg[len++] = usb.get();
					if(len >= Modbus_ASCII::MSG_LEN) {
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
};

};

#endif