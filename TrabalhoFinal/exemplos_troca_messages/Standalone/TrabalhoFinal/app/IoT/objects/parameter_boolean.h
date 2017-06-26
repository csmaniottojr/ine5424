#ifndef parameter_boolean_h__
#define parameter_boolean_h__

#include "parametertype.h"


using namespace EPOS;

namespace IoT {

    class ParameterBoolean : public ParameterType
    {
    public:
        static const unsigned char VALUE_SIZE = sizeof (bool );
    private:
        bool * _data;
    public:

        ParameterBoolean ( ) {
            _update = new Callback ( );
            _data = 0;
        }

        ParameterBoolean ( bool * data )
        : ParameterType ( ), _data ( data ) {
            setMinValue ( false );
            setMaxValue ( true );
            _update = new Callback ( ); //Does nothing!
            this->_type = BOOLEAN;

        }

        ParameterBoolean ( Callback * update, bool * data )
        : ParameterType ( ), _data ( data ) {
            setMinValue ( false );
            setMaxValue ( true );
            this->_update = update;
            this->_type = BOOLEAN;
        }

        /* Setters */
        void setMinValue ( bool min ) {

            if ( _min != 0 )
                delete _min;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( bool* ) & value[1] )[0] = min;
            _min = value;
        }

        void setMaxValue ( bool max ) {
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

        const bool * getData ( ) {
            return _data;
        }

    } ;
}

#endif