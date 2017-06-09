#include "messages.h"
#include <machine.h>

using namespace EPOS;

//OStream cout;

int main(){
    RegisterServiceRequest request("Configuracao");
    RegisterServiceRequest requestParsed;
    while(1){
        cout << "Size class: " << request.size() << "/" << sizeof(RegisterServiceRequest) << endl;
        cout << "Type class: " << request.type() << endl;
        cout << "Id class:";
        for(unsigned char i = 0; i<4; i++)
            cout << hex << " " << request.id()[i];
        cout << endl;
        cout << "ServiceName class: " << request.serviceName() << endl << endl;

        const char * msg = request.toString();
        cout << "Message:";
        for(unsigned int i = 0; i<=request.size(); i++)
            cout << " " << hex << (unsigned char)msg[i];
        cout << endl << endl;

        requestParsed.parseMessage(msg);

        cout << "Size parse: " << requestParsed.size() << "/" << sizeof(RegisterServiceRequest) << endl;
        cout << "Type parse: " << requestParsed.type() << endl;
        cout << "Id parse:";
        for(unsigned char i = 0; i<4; i++)
            cout << hex << " " << requestParsed.id()[i];
        cout << endl;
        cout << "ServiceName parse: " << requestParsed.serviceName(); 
        cout << endl << "=====================" << endl;

        Machine::delay(7000000);
    }
}