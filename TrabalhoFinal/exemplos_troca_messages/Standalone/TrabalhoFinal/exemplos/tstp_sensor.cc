#include <machine.h>
#include <thread.h>
#include <smart_data.h>
#include "cheats/led.h"
#include <alarm.h>
using namespace EPOS;

OStream cout;

int main()
{
    Alarm::delay(5*1000000);//pra dar tempo de conectar
    eMoteIII::led::blink(0.5,2);
    cout << "TSTP Sensor test" << endl;


    cout << "My machine ID is:";
    for(unsigned int i = 0; i < 8; i++)
        cout << " " << hex << Machine::id()[i];
    cout << endl;
    cout << "You can set this value at src/component/tstp_init.cc to set initial coordinates for this mote." << endl;

    cout << "My coordinates are " << TSTP::here() << endl;
    cout << "The time now is " << TSTP::now() << endl;
    cout << "I am" << (TSTP::here() == TSTP::sink() ? " " : " not ") << "the sink" << endl;

                    
    Acceleration a(0, 1000000, Acceleration::ADVERTISED);

    Thread::self()->suspend();

    return 0;
}
