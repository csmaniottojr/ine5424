#ifndef parameter_boolean_h__
#define parameter_boolean_h__

#include "parametertype.h"


using namespace EPOS;

namespace IoT {

    class Parameter_Boolean : public Parameter_Type
    {
    public:
        static const unsigned char VALUE_SIZE = sizeof (bool );
    private:
        bool * _data;
    public:

        Parameter_Boolean ( bool * data )
        : Parameter_Type ( ), _data ( data ) {
            min ( false );
            max ( true );
            _update = new Callback ( ); //Does nothing!
            this->_type = BOOLEAN;

        }

        Parameter_Boolean ( Callback * update, bool * data )
        : Parameter_Type ( ), _data ( data ) {
            min ( false );
            max ( true );
            this->_update = update;
            this->_type = BOOLEAN;
        }

        /* Setters */
        void min ( bool min ) {

            if ( _min != 0 )
                delete _min;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( bool* ) & value[1] )[0] = min;
            _min = value;
        }

        void max ( bool max ) {
            if ( _max != 0 )
                delete _max;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( bool* ) & value[1] )[0] = max;
            _max = value;
        };

        void update ( const bool data ) {
            *this->_data = data;
            _update->operator () ( );
        }

        
    } ;
}

#endif