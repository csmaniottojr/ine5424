//Cheat utility to control a eMoteIII led.
#include <gpio.h>
#include <alarm.h>

using namespace EPOS;
    namespace eMoteIII{
        namespace led{
        //Turns led On
        inline void ledOn() {GPIO('C',3, GPIO::OUT).set(1);}
        //Turns led Off
        inline void ledOff() {GPIO('C',3, GPIO::OUT).set(0);}
        inline void blink(float seconds,unsigned int times=1) {
        //Bliks times times led in time*seconds
            while(times>0){
                GPIO('C',3, GPIO::OUT).set(1);
                Alarm::delay(seconds*1000000); //It's in microseconds
                GPIO('C',3, GPIO::OUT).set(0);
                Alarm::delay(seconds*1000000); //It's in microseconds / if in a loop
                times--;
            }
        }
    }
}
