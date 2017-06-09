#include "messages.h"
#include <nic.h>
#include <ieee802_15_4.h>
#include <machine.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

class Receiver : public IEEE802_15_4::Observer
{
public:
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;

    Receiver(const Protocol & p, NIC * nic) : _prot(p), _nic(nic) {
        _nic->attach(this, _prot);
        memcpy(_myId, &Machine::id()[4], 4);
    }

    void update(Observed * o, Protocol p, Buffer * b) {
        cout << "Mensagem recebida!" << endl;
        
        if(memcmp(b->frame()->data<Message>()->id(), _myId, 4) == 0){
            if(b->frame()->data<RegisterResponse>()->valid()){
                RegisterResponse *response = b->frame()->data<RegisterResponse>();
                cout << "RegisterResponse recebido!" << endl;
                cout << "Devo mandar os dados: " << response->sendData() << endl;

                if(response->sendData()){
                    cout << "Mandando servico de configuracao..." << endl;
                    RegisterServiceRequest request("Configuracao");
                    _nic->send(b->frame()->src(), p, &request, sizeof(RegisterServiceResponse));
                }
            }else if(b->frame()->data<RegisterServiceResponse>()->valid()){
                cout << "RegisterServiceResponse recebido!" << endl;
            }
            cout << endl;
        }
        _nic->free(b);
    }

private:
    Protocol _prot;
    NIC * _nic;
    unsigned char _myId[4];
};

int main(){
    NIC * nic = new NIC();
    Receiver r(IEEE802_15_4::ELP, nic);

    cout << "Object esta enviando RegisterRequests e esta esperando RegisterResponses!" << endl;
    cout << "Machine::id() = ";
    for(unsigned int i = 0; i < 8; i++)
        cout << " " << hex << Machine::id()[i];
    cout << endl << endl;

    RegisterRequest rr;
    while(1){
        cout << "Enviando broadcast do RegisterRequest!" << endl;
        cout << "   Type: " << rr.type() << endl;
        cout << "   My Id = ";
        for(unsigned int i = 0; i<4; i++)
            cout << " " << hex << rr.id()[i];
        cout << endl;

        nic->send(nic->broadcast(), IEEE802_15_4::ELP, &rr, sizeof(RegisterRequest));
        Machine::delay(7000000);
    }
}
