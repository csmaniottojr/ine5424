#include "IoT/objects/callback.h"
#include "cheats/led.h"
#include "framework/main.h"
#include "IoT/objects/parameter.h"
#include "IoT/objects/parameter_boolean.h"


using namespace EPOS;
using namespace IoT;
EPOS::OStream cout;

void func( ) {
    cout << "Func called!\n";
}
//Data variables
bool * myBool = new bool( true );
float * myFloat = new float( 0 );
int * current_combo = new int ( 0 );

int main( ) {
    cout << "Initializing boolean tests\n";
    eMoteIII::led::blink( 0.1, 10 );
    EPOS::Alarm::delay( 2 * 1000000 );
    //BOOLEAN TESTS
    Callback update( &func );
    Parameter_Boolean * x = new Parameter_Boolean( &update, myBool );
    cout << "Created a bool paramType" << "\n";

    Parameter * ligado = new Parameter( "Ligado", 1, x );
    cout << "Created a parameter [" << ligado->name( ) << "]\n";

    cout << ligado->name( ) << " is " << *myBool << "\n";
    cout << "Updating " << ligado->name( ) << " with false\n";

    ligado->update( false );
    cout << ligado->name( ) << " is " << *myBool << "\n";
    eMoteIII::led::blink( 2 );

    //FLOAT TESTS
    cout << "Initializing float tests\n";
    eMoteIII::led::blink( 0.1, 10 );
    EPOS::Alarm::delay( 2 * 1000000 );

    Parameter_Float * y = new Parameter_Float( &update, myFloat, 0.0, 100.0 );
    cout << "Created a float paramType" << "\n";

    Parameter * temp = new Parameter( "Temperature", 1, y );
    cout << "Created a parameter [" << ligado->name( ) << "]\n";

    cout << temp->name( ) << "is" << *myFloat << "\n";
    cout << "Updating " << temp->name( ) << " with 10\n";

    temp->update( ( float ) 10.0 );
    cout << temp->name( ) << "is" << *myFloat << "\n";
    eMoteIII::led::blink( 2 );

    //COMBO TEST
    cout << "Initializing combo tests\n";
    eMoteIII::led::blink( 0.1, 10 );
    EPOS::Alarm::delay( 2 * 1000000 );

    Parameter_Combo * z = new Parameter_Combo( &update, current_combo );
    cout << "Created a combo paramType\n";

    Parameter * mode = new Parameter( "Mode", 1, z );
    cout << "Created a parameter [" << mode->name( ) << "]\n";

    cout << "Populating combo with low|normal|max\n";
    int indexes[3];
    indexes[0] = mode->insert_combo( "low" );
    cout << "Inserted " << mode->get_combo( indexes[0] ) << " - " << indexes[0] << endl;
    indexes[1] = mode->insert_combo( "normal" );
    cout << "Inserted " << mode->get_combo( indexes[1] ) << " - " << indexes[1] << endl;
    indexes[2] = mode->insert_combo( "max" );
    cout << "Inserted " << mode->get_combo( indexes[2] ) << " - " << indexes[2] << endl;

    cout << "Current option is " << *current_combo << " - " << mode->get_combo( *current_combo ) << endl;
    cout << "updating current option index with " << indexes[2] << endl;
    mode->update( indexes[2] );
    cout << "Current option is " << *current_combo << " - " << mode->get_combo( *current_combo ) << endl;
    
    eMoteIII::led::blink( 2 );



}