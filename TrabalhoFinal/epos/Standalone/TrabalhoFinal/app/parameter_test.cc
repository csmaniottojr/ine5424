#include "IoT/objects/callback.h"
#include "cheats/led.h"
#include "framework/main.h"
#include "IoT/objects/parameter.h"
#include "IoT/objects/parameter_boolean.h"


using namespace EPOS;
using namespace IoT;
EPOS::OStream cout;

void func( ) {
    cout << "My bool is " << *myBool << endl;
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
    //BOOLEAN TESTS-------------------------------------------------------------
    Callback update( &func );
    ParameterBoolean * x = new ParameterBoolean( &update, myBool );
    cout << "Created a bool paramType" << "\n";

    Parameter * ligado = new Parameter( "Ligado", 1, x );
    cout << "Created a parameter [" << ligado->getName( ) << "]\n";

    cout << ligado->getName( ) << " is " << *myBool << "\n";
    cout << "Updating " << ligado->getName( ) << " with false\n";

    ligado->update( false );
    cout << ligado->getName( ) << " is " << *myBool << "\n";
    eMoteIII::led::blink( 2 );

    //FLOAT TESTS---------------------------------------------------------------
    cout << "Initializing float tests\n";
    eMoteIII::led::blink( 0.1, 10 );
    EPOS::Alarm::delay( 2 * 1000000 );

    ParameterFloat * y = new ParameterFloat( &update, myFloat, 0.0, 100.0 );
    cout << "Created a float paramType" << "\n";

    Parameter * temp = new Parameter( "Temperature", 1, y );
    cout << "Created a parameter [" << ligado->getName( ) << "]\n";

    cout << temp->getName( ) << "is" << *myFloat << "\n";
    cout << "Updating " << temp->getName( ) << " with 10\n";

    temp->update( ( float ) 10.0 );
    cout << temp->getName( ) << "is" << *myFloat << "\n";
    eMoteIII::led::blink( 2 );

    //COMBO TEST----------------------------------------------------------------
    cout << "Initializing combo tests\n";
    eMoteIII::led::blink( 0.1, 10 );
    EPOS::Alarm::delay( 2 * 1000000 );

    ParameterCombo * z = new ParameterCombo( &update, current_combo );
    cout << "Created a combo paramType\n";

    Parameter * mode = new Parameter( "Mode", 1, z );
    cout << "Created a parameter [" << mode->getName( ) << "]\n";

    cout << "Populating combo with low|normal|max\n";
    int indexes[3];
    indexes[0] = mode->insertCombo( "low" );
    cout << "Inserted " << mode->getCombo( indexes[0] ) << " - " << indexes[0] << endl;
    indexes[1] = mode->insertCombo( "normal" );
    cout << "Inserted " << mode->getCombo( indexes[1] ) << " - " << indexes[1] << endl;
    indexes[2] = mode->insertCombo( "max" );
    cout << "Inserted " << mode->getCombo( indexes[2] ) << " - " << indexes[2] << endl;

    cout << "Current option is " << *current_combo << " - " << mode->getCombo( *current_combo ) << endl;
    cout << "updating current option index with " << indexes[2] << endl;
    mode->update( indexes[2] );
    cout << "Current option is " << *current_combo << " - " << mode->getCombo( *current_combo ) << endl;

    eMoteIII::led::blink( 2 );



}