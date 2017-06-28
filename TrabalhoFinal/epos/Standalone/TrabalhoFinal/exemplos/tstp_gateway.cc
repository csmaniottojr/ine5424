#include <machine.h>
#include <alarm.h>
#include <smart_data.h>
#include <tstp.h>
#include "cheats/led.h"

using namespace EPOS;

OStream cout;

typedef TSTP::Coordinates Coordinates;
typedef TSTP::Region Region;

const unsigned int INTEREST_PERIOD = 1000000;
const unsigned int INTEREST_EXPIRY = 2 * INTEREST_PERIOD;
const unsigned int MINICOM_WAIT = 5 * INTEREST_PERIOD;

int main()
{
    Alarm::delay(MINICOM_WAIT);
    eMoteIII::led::blink(0.5,5);
    cout << "TSTP Gateway test" << endl;
//Printa o id da maquina
    cout << "My machine ID is:";
    for(unsigned int i = 0; i < 8; i++)
        cout << " " << hex << Machine::id()[i];
    cout << endl;
    cout << "You can set this value at src/component/tstp_init.cc to set initial coordinates for this mote." << endl;
//Printa as coordenadas relativas do eMote
    cout << "My coordinates are " << TSTP::here() << endl;
    cout << "The time now is " << TSTP::now() << endl;
    cout << "I am" << (TSTP::here() == TSTP::sink() ? " " : " not ") << "the sink" << endl;
    //Coordenadas do centro do sensor
    Coordinates center_sensor(0,0,0);
    //                   Centro        raio    desde     até
    Region region_sensor(center_sensor, 20, TSTP::now(), -1);

    cout << "I will now ask for Acceleration data from any sensor located in " << region_sensor << endl;
    //Registra interesse em uma aceleração
    Acceleration acceleration(region_sensor, INTEREST_EXPIRY, INTEREST_PERIOD);

    while(true) {
        Alarm::delay(INTEREST_PERIOD);
        cout << "Acceleration in " << acceleration.location() << " at " << acceleration.time() << " was " << acceleration << endl;
    }

    return 0;
}
