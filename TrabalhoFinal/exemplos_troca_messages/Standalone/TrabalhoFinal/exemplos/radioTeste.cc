#include <utility/ostream.h>
#include <nic.h>
#include <gpio.h>
#include <alarm.h>
#include "cheats/led.h"

using namespace EPOS;

OStream cout;

void sender() {

    char mensagem[] = "     Ola mundo";
    void * data = mensagem;
    int tamanho = sizeof (mensagem) - 4 * sizeof (char);
    int * sizeMod = (int*) data;
    sizeMod[0] = tamanho;

    NIC * nic = new NIC();
    const unsigned int delay_time = 250000;
    cout << "Hello, I am the sender." << endl;
    cout << "I will send a message every " << delay_time << " microseconds." << endl;
    while (1) {
        eMoteIII::led::blink(0.1, 2);
        cout << "Sending message: " << mensagem << endl;
        nic->send(nic->broadcast(), NIC::ELP, data, ((sizeof (char)*(sizeof (mensagem))) + 1 * sizeof (char)));
        cout << "Sent" << endl;
        mensagem[4] = ((mensagem[4] - '0' + 1) % 10) + '0';
        Alarm::delay(delay_time);
    }
}

bool led_value;
GPIO * led;

class Receiver : public IEEE802_15_4::Observer {
    typedef char data_type;

public:
    typedef IEEE802_15_4::Protocol Protocol;
    typedef IEEE802_15_4::Buffer Buffer;
    typedef IEEE802_15_4::Frame Frame;
    typedef IEEE802_15_4::Observed Observed;

    Receiver(const Protocol & p, NIC * nic) : _prot(p), _nic(nic) {
        _nic->attach(this, _prot);
    }

    void update(Observed * o, Protocol p, Buffer * b) {
        cout << "Received buffer" << reinterpret_cast<void *> (b) << endl;
        if (p == _prot) {
            eMoteIII::led::blink(0.1, 1);
            Frame * f = b->frame();
            auto d = f->data<data_type>();
            cout << endl << "=====================" << endl;
            cout << "Received " << b->size() << " bytes of payload from " << f->src() << " :" << endl;
            //Four first bytes contains the message size!
            int * size = (int*) d;
            cout << "Message has " << *size << "bytes!" << endl; //size+message
            for (int i = 4; /*Size of int*/ i < *size + 3; /*Forgive about \0*/ i++)
                cout << d[i] << "";
            cout << endl << "=====================" << endl;
            _nic->free(b);
        }
    }

private:
    Protocol _prot;
    NIC * _nic;
};

void receiver() {
    led = new GPIO('C', 3, GPIO::OUT);
    led_value = true;
    led->set(led_value);
    cout << "Hello, I am the receiver." << endl;
    cout << "I will attach myself to the NIC and print every message I get." << endl;
    NIC * nic = new NIC();
    Receiver * r = new Receiver(NIC::ELP, nic);
}

int main() {
    cout << "Hello main" << endl;

    //receiver();
    //sender();

    while (1) {
        for (unsigned int i = 0; i < 8; i++)
            cout << " " << hex << Machine::id()[i];
        cout << endl;
        Alarm::delay(1000000);
    }

    return 0;
}
