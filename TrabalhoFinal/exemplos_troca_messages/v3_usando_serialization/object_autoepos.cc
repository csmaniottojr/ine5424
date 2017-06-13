#include "messages.h"
#include "serialization.h"
#include "cheats/led.h"
#include <machine.h>

using namespace EPOS;
//OStream cout;

//a b0 d 6
//c8 ab d 6

class Receiver : public IEEE802_15_4::Observer
{
public:
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;

    Receiver(const Protocol & p, NIC * nic) 
    : _prot(p), _nic(nic) {
        _nic->attach(this, _prot);
        memcpy(_myId, &Machine::id()[RegisterMessage::ID_SIZE], RegisterMessage::ID_SIZE);
    }

    void update(Observed * o, Protocol p, Buffer * b) {
        eMoteIII::led::blink(0.1, 2);
        auto msg = b->frame()->data<char>();

        cout << "Received " << b->size() << " bytes of payload from " << b->frame()->src() << " :";
        for(int i=0; i<b->size(); i++)
            cout << " " << hex << (unsigned char) msg[i];
        cout << dec << endl;

        RegisterMessage * message = Serialization::deserialize(msg);

        if(memcmp(message->id(), _myId, RegisterMessage::ID_SIZE) == 0){
            if(message->type() == RegisterMessage::REGISTER_RESPONSE){
                cout << "   Type: RegisterResponse!" << endl;
                RegisterResponse *resp = reinterpret_cast<RegisterResponse*>(message);
                cout << "   SendData: " << resp->sendData() << endl;

                if(resp->sendData()){
                    cout << "Mandando servico de configuracao..." << endl;
                    RegisterServiceRequest request("Configuracao");

                    auto msg = Serialization::serialize(&request);
                    _nic->send(b->frame()->src(), IEEE802_15_4::ELP, msg, request.size()+2);
                }
            }else if(message->type() == RegisterMessage::REGISTER_SERVICE_RESPONSE){
                cout << "   Type: RegisterServiceResponse!" << endl;

                cout << "Mandando parametro temperatura [valor inteiro]..." << endl;
                RegisterIntParameterRequest request("Temperatura", 15, 25);

                auto msg = Serialization::serialize(&request);
                _nic->send(b->frame()->src(), IEEE802_15_4::ELP, msg, request.size()+2);
            }else if(message->type() == RegisterMessage::REGISTER_PARAM_RESPONSE){
                cout << "   Type: RegisterParamResponse!" << endl;
            }
            cout << endl;
        }

        _nic->free(b);
        delete message;
    }
private:
    Protocol _prot;
    NIC * _nic;
    
    RegisterMessage::ID _myId;
};

int main(){
    cout << "Machine::id() = ";
    for(unsigned int i = 0; i < 8; i++)
        cout << " " << hex << Machine::id()[i];
    cout << endl << endl;

    NIC * nic = new NIC();
    Receiver r(IEEE802_15_4::ELP, nic);

    cout << "Object esta enviando requests e esta esperando responses!" << endl;

    RegisterRequest rr;
    cout << "RegisterRequest::id() = ";
    for(unsigned int i = 0; i < RegisterMessage::ID_SIZE; i++)
        cout << " " << hex << rr.id()[i];
    cout << endl << endl;

    while(1){
        eMoteIII::led::blink(0.1, 1);
        cout << "Enviando broadcast do RegisterRequest! [" << rr.size() << "]" << endl;
        
        auto msg = Serialization::serialize(&rr);
        cout << "Msg:";
        for(int i=0; i<strlen(msg); i++)
            cout << " " << hex << (unsigned char) msg[i];
        cout << endl;
        nic->send(nic->broadcast(), IEEE802_15_4::ELP, msg, rr.size()+2);

        Machine::delay(10000000);
        delete msg;
    }
}