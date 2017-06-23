#include "cheats/led.h"
#include <machine.h>
#include <alarm.h>
#include <thread.h>
#include <usb.h>

#include "IoT/usb_manager.h"
#include "IoT/iot_gateway.h"
#include "IoT/messages/register_serialization.h"
#include "IoT/messages/command_serialization.h"
#include "IoT/objects/smartobject.h"
#include "IoT/objects/parameter_float.h"

using namespace EPOS;
using namespace IoT;

OStream cout;

//IDs: 101559240 e 101560330 
int main(){
    NIC * nic = new NIC();
    IoT::IotGateway gateway(IEEE802_15_4::ELP, nic);
    cout << "# Inicializando o IoT Gateway..." << endl;
    cout << "# My ID: " << *((SmartObject::ID*) &Machine::id()[4]) << endl << endl;

    Thread * thread = new Thread(&USBManager::run, &gateway);
    while(1){
        Alarm::delay(10000000);
    }

    /*Alarm::delay(2000000);
    char msg[] = {59,29,200,171,13,6,7,5,3,0,0,0,128,63,0,0,0,64,84,101,109,112,101,114,97,116,117,114,97,15,236,125,253,186,248,247,220,199,237,239};

    while(1){
        eMoteIII::led::blink(0.1, 1);
        USBManager::send(msg, ((unsigned char)msg[1]));
        Alarm::delay(10000000);
    }*/

    /* Testes ...

    // Converte o dado pra Data*
    float value = 15.00;
    CommandMessage::Data * data = new CommandMessage::Data[sizeof(float)];
    ((float*) data)[0] = value;

    CommandMessage message(2, data, sizeof(float), CommandMessage::COMMAND_WRITE_REQUEST);
    auto msgSerialized = CommandSerialization::serialize(&message);
    CommandMessage * msgDeserialized = CommandSerialization::deserialize(msgSerialized);
    while(1){
        cout << "Msg normal: " << endl;
        cout << "   Size: " << message.getSize() << endl;
        cout << "   ID: " << message.getId() << endl;
        cout << "   regId: " << message.getRegisterId() << endl;
        cout << "   Data: ";
        for(int i = 0; i<message.getDataLength(); i++)
            cout << (unsigned char) message.getData()[i] << " ";
        cout << endl;
        cout << "   Data Converted: " << *((float*) message.getData()) 
            << endl << endl;

        cout << "Msg serialized: ";
        for(int i = 0; i<((unsigned char)msgSerialized[1]); i++)
            cout << (unsigned char) msgSerialized[i] << " ";
        cout << endl << endl;

        cout << "Msg deserialized:" << endl;
        cout << "   Size: " << msgDeserialized->getSize() << endl;
        cout << "   ID: " << msgDeserialized->getId() << endl;
        cout << "   regId: " << msgDeserialized->getRegisterId() << endl;
        cout << "   Data: ";
        for(int i = 0; i<msgDeserialized->getDataLength(); i++)
            cout << (unsigned char) msgDeserialized->getData()[i] << " ";
        cout << endl;
        cout << "   Data Converted: " << *((float*) msgDeserialized->getData()) 
            << endl << endl;

        Alarm::delay(10000000);
    }

    */
}