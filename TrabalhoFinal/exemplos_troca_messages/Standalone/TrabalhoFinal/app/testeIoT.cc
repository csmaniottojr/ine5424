#include <utility/ostream.h>
#include "so2/objects.h"
#include <gpio.h>
#include <alarm.h>
#include "cheats/led.h"
#include "framework/main.h"

//REMOVE THIS AFTER DEBUG FOR GOD SAKE!
#define DEBUG_BUILD

using namespace EPOS;


EPOS::OStream cout;

int main( ) {

    eMoteIII::led::blink( 0.1, 5 );
    EPOS::Alarm::delay( 1000000 );

    //Testing parameter creation
    cout << "=====BOOL PARAMETER TEST!=====\n";
    EPOS::Alarm::delay( 2000000 );
    Parameter new_parameter
            ( ( char* ) "ParTeste1", Parameter::Param_Type::BOOLEAN );

    cout << "Created a parameter called " << new_parameter.name( ) << "\n";
    new_parameter.bool_data( true );
    cout << "Parametro " << new_parameter.name( ) << " é " <<
            new_parameter.bool_data( ) << "\n";
    new_parameter.bool_data( false );
    cout << "Parametro " << new_parameter.name( ) << " é " <<
            new_parameter.bool_data( ) << "\n";

    cout << "=====COMBO PARAMETER INSERTION TEST!=====\n";
    EPOS::Alarm::delay( 2000000 );
    //Testing combo insertion
    Parameter combo_parameter
            ( ( char* ) "ParCombo", Parameter::Param_Type::COMBO );
    char* combo_list[] = { "Ligado", "Desligado", "StandBy" };
    int combo_id[3];
    for ( int i = 0; i < 3; i++ ) {
        combo_id[i] = combo_parameter.insert_combo( combo_list[i] );
        cout << "Inserted combo " << combo_list[i] << "\n";
    }

    cout << "=====COMBO PARAMETER RECOVER TEST!=====\n";
    EPOS::Alarm::delay( 2000000 );
    cout << "We have " << combo_parameter.combo_counter( ) << " combos!\n";
    for ( int combo_index = 0; combo_index < combo_parameter.combo_counter( ); combo_index++ ) {
        cout << "The combo number " << combo_id[combo_index] << " is " << combo_parameter.combo( combo_id[combo_index] ) << "\n";
    }

    cout << "=====SERVICE TEST!=====\n";
    EPOS::Alarm::delay( 2000000 );
    //Testing parameter insertion into Service
    Service new_service( ( char* ) "SerTeste1" );
    cout << "Created a service called " << new_service.name( ) << "\n";
    new_service.insert_parameter( new_parameter );
    cout << "Inserted into " << new_service.name( ) << " the parameter" <<
            new_parameter.name( ) << " = " << new_parameter.bool_data( ) << "\n";

    return 0;
}