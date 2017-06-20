#include "cheats/led.h"
#include <machine.h>
#include <alarm.h>
#include <thread.h>

#include "IoT/usb_manager.h"
#include "IoT/iot_gateway.h"
#include "IoT/messages/serialization_register.h"
#include "IoT/objects/smartobject.h"
#include "IoT/objects/parameter_float.h"

using namespace EPOS;
using namespace IoT;

OStream cout;

int main(){
    NIC * nic = new NIC();
    IoT::IotGateway gateway(IEEE802_15_4::ELP, nic);

    //TODO Ta com problema... quando inicia soh o Gateway e tu manda msg por python
    // tudo funciona, mas caso o Object tambem esteja ativo o Gateway para
    // de reconhecer as msgs vindas do codigo python...
    Thread * thread = new Thread(&USBManager::run, &gateway);

    cout << "Inicializando o IoT Gateway..." << endl << endl;
    while(1){
        eMoteIII::led::blink(0.1, 1);

        Alarm::delay(10000000);
    }

    
    
    
    
    /* Testes...
    
    ParameterFloat _temp(1, 2);
    Parameter temp("Temperatura", 2, &_temp);

    RegisterParameterRequest req(&temp);
    auto msg = SerializationRegister::serialize(&req);
    RegisterParameterRequest *req2 = 
        reinterpret_cast<RegisterParameterRequest*>(SerializationRegister::deserialize(msg));
    while(1){
        cout << "Req normal:" << endl;
        cout << "   name: " << req.getParameterName() << endl;
        cout << "   regId: " << req.getRegisterId() << endl;

        const char * value = req.getMinValue();
        cout << "   min: ";
        for(int i = 1; i<=((unsigned char)value[0]); i++)
            cout << hex << (unsigned char) value[i] << " ";
        cout << dec;
        cout << "/ " << (*((float*) &value[1])) << endl;

        value = req.getMaxValue();
        cout << "   max: ";
        for(int i = 1; i<=((unsigned char)value[0]); i++)
            cout << hex << (unsigned char) value[i] << " ";
        cout << dec;
        cout << "/ " << (*((float*) &value[1])) << endl << endl;

        //===============================================

        cout << "Message serialized [" << ((unsigned char) msg[0]) << "]:";
        for(int i=0; i<((unsigned char) msg[0]); i++)
            cout << " " << hex << (unsigned char) msg[i];
        cout << dec << endl;

        cout << "Req deserialized:" << endl;
        cout << "   name: " << req2->getParameterName() << endl;
        cout << "   regId: " << req2->getRegisterId() << endl;

        value = req2->getMinValue();
        cout << "   min: ";
        for(int i = 1; i<=((unsigned char)value[0]); i++)
            cout << hex << (unsigned char) value[i] << " ";
        cout << dec;
        cout << "/ " << (*((float*) &value[1])) << endl;

        value = req2->getMaxValue();
        cout << "   max: ";
        for(int i = 1; i<=((unsigned char)value[0]); i++)
            cout << hex << (unsigned char) value[i] << " ";
        cout << dec;
        cout << "/ " << (*((float*) &value[1])) << endl << endl;

        cout << endl;

        Alarm::delay(7000000);
    }*/
}