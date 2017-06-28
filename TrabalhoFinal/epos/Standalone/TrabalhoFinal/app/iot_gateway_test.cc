#include "cheats/led.h"
#include <machine.h>
#include <alarm.h>
#include <thread.h>

#include "IoT/usb_manager.h"
#include "IoT/iot_gateway.h"
#include "IoT/messages/register_serialization.h"
#include "IoT/objects/smartobject.h"
#include "IoT/objects/parameter_float.h"
#include "IoT/messages/command_message.h"
#include "IoT/messages/command_serialization.h"

using namespace EPOS;
using namespace IoT;

OStream cout;

int main(){
    NIC * nic = new NIC();
    IoT::IotGateway gateway(IEEE802_15_4::ELP, nic);
    Thread * thread = new Thread(&USBManager::run, &gateway);

    cout << "Inicializando o IoT Gateway..." << endl << endl;
    while(1){
        eMoteIII::led::blink(0.1, 1);

        Alarm::delay(10000000);
    }
}