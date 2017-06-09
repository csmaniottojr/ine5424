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

    typedef unsigned char STATE;
    enum {
        WAITING_REGISTER = 0,
        WAITING_SERVICE = 1,
        WAITING_PARAMETER = 2,
        WAITING_COMBO = 3
    };

    Receiver(const Protocol & p, NIC * nic) 
    : _prot(p), _nic(nic), _state(WAITING_REGISTER) {
        _nic->attach(this, _prot);
    }

    void update(Observed * o, Protocol p, Buffer * b) {
        cout << "Mensagem recebida!" << endl;

        if(_state == WAITING_REGISTER && b->frame()->data<RegisterRequest>()->valid()){
            RegisterRequest *request = b->frame()->data<RegisterRequest>();
            cout << "RegisterRequest recebido do ID:";
            for(unsigned int i = 0; i<4; i++)
                cout << " " << hex << request->id()[i];
            cout << endl;

            Machine::delay(1000000);//verifica no banco de dados
            cout << "Mandando ACK!" << endl;
            _rResp.setId(request->id());
            _rResp.setSendData(true);
            _nic->send(b->frame()->src(), p, &_rResp, sizeof(RegisterResponse));

            _state = WAITING_SERVICE;
            memcpy(_currentId, request->id(), 4);
        }else if(_state == WAITING_SERVICE && 
            b->frame()->data<RegisterServiceRequest>()->valid() &&
            memcmp(b->frame()->data<RegisterServiceRequest>()->id(), _currentId, 4) == 0){
            
            RegisterServiceRequest *request = b->frame()->data<RegisterServiceRequest>();
            cout << "RegisterServiceRequest recebido do ID:";
            for(unsigned int i = 0; i<4; i++)
                cout << " " << hex << request->id()[i];
            cout << endl;

            cout << "Size: " << request->size() << endl;
            cout << "Service Name: ";
            for(unsigned int i = 0; i<request->size(); i++)
                cout << request->serviceName()[i] << "";
            cout << endl;

            cout << "Mandando ACK!" << endl;
            _rsResp.setId(request->id());
            _nic->send(b->frame()->src(), p, &_rsResp, sizeof(RegisterServiceResponse));

            _state = WAITING_REGISTER;//por enquanto volta pro inicio...
        }
        _nic->free(b);
    }

private:
    RegisterResponse _rResp;
    RegisterServiceResponse _rsResp;

    Protocol _prot;
    NIC * _nic;

    unsigned char _currentId[4];
    STATE _state;
};

int main(){
    NIC * nic = new NIC();
    Receiver r(IEEE802_15_4::ELP, nic);

    cout << "Gateway esta esperando RegisterRequests e esta enviando RegisterResponses!" << endl;
    cout << "Machine::id() = ";
    for(unsigned int i = 0; i < 8; i++)
        cout << " " << hex << Machine::id()[i];
    cout << endl;

    RegisterServiceRequest req("Configuracao");
    cout << "SizeOf: " << sizeof(RegisterServiceRequest) << endl;
    cout << "Type: " << req.type() << endl;
    cout << "Valid: " << req.valid() << endl << endl;
    cout << "Size: " << req.size() << endl;
    cout << "ServiceName: " << req.serviceName() << endl;

    while(1);
}
