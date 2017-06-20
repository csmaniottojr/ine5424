#include "IoT/objects/callback.h"
#include "cheats/led.h"
#include "framework/main.h"
using namespace EPOS;
EPOS::OStream cout;

void func( ) {
    cout << "Func called!\n";
}

int main( ) {
    eMoteIII::led::blink( 0.1, 10 );
    EPOS::Alarm::delay( 2 * 1000000 );
    Callback * test = new Callback( );
    test->set_callback( &func );
    test->operator ()( );

}