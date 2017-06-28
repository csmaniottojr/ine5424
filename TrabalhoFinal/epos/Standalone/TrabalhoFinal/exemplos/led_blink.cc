#include <gpio.h>
#include <alarm.h>
#include "cheats/led.h"
#include <utility/random.h>
#include <utility/ostream.h>
#include <i2c.h>
using namespace EPOS;


#define SECOND 1000000
OStream cout;
I2C_Temperature_Sensor ts;
I2C_Humidity_Sensor hs;
int main()
{
    while(true)
    {
        int blink_times = Random::random() % 10;
        cout << "Blink " << blink_times <<   " times!\n";
        cout << "There are " << ts.get() << "C\n";
        while (blink_times > 0)
        {
            eMoteIII::led::blink(0.2);
            blink_times--;
        }
        if (blink_times==0) Alarm::delay(2*SECOND);
    }
    return 0;
}
