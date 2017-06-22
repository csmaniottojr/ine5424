#include "cheats/led.h"
#include <machine.h>
#include <alarm.h>
#include <thread.h>
#include <usb.h>

#include "IoT/usb_manager.h"
#include "IoT/iot_gateway.h"
#include "IoT/messages/serialization_register.h"
#include "IoT/messages/command_serialization.h"
#include "IoT/objects/smartobject.h"
#include "IoT/objects/parameter_float.h"

using namespace EPOS;
using namespace IoT;

OStream cout;

int main(){
    NIC * nic = new NIC();
    IoT::IotGateway gateway(IEEE802_15_4::ELP, nic);
    cout << "Inicializando o IoT Gateway..." << endl << endl;

    Thread * thread = new Thread(&USBManager::run, &gateway);
    while(1){
        Alarm::delay(10000000);
    }

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
            cout << hex << (unsigned char) message.getData()[i] << " ";
        cout << dec << endl;
        cout << "   Data Converted: " << *((float*) message.getData()) 
            << endl << endl;

        cout << "Msg serialized: ";
        for(int i = 0; i<=((unsigned char)msgSerialized[1]); i++)
            cout << hex << (unsigned char) msgSerialized[i] << " ";
        cout << dec << endl << endl;

        cout << "Msg deserialized:" << endl;
        cout << "   Size: " << msgDeserialized->getSize() << endl;
        cout << "   ID: " << msgDeserialized->getId() << endl;
        cout << "   regId: " << msgDeserialized->getRegisterId() << endl;
        cout << "   Data: ";
        for(int i = 0; i<msgDeserialized->getDataLength(); i++)
            cout << hex << (unsigned char) msgDeserialized->getData()[i] << " ";
        cout << dec << endl;
        cout << "   Data Converted: " << *((float*) msgDeserialized->getData()) 
            << endl << endl;

        Alarm::delay(10000000);
    }

    */
}