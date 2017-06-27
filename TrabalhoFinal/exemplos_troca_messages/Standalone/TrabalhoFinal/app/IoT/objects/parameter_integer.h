#ifndef parameter_integer_h__
#define parameter_integer_h__

#include "callback.h"
#include "parametertype.h"


using namespace EPOS;

namespace IoT {

    class ParameterInteger : public ParameterType
    {
    public:
        static const unsigned char VALUE_SIZE = sizeof (int );
    private:
        int * _data;

    public:
        ParameterInteger ( )
        : ParameterType ( ), _data ( 0 ) {
            this->_update = new Callback ( ); //Does nothing!
            this->_type = INTEGER;
        }

        ParameterInteger ( Callback * update ,  int * data, int min_, int max_ )
        : ParameterType ( ) , _data ( data ) {
            setMinValue ( min_ );
            setMaxValue ( max_ );
            this-> _update  = ( update );
            this->_type = INTEGER;
        }

        /* Setters */
        void setMinValue ( int min ) {
            if ( _min != 0 )
                delete[] _min;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( int* ) &value[1] )[0] = min;
            _min = value;
        }

        void setMaxValue ( int max ) {
            if ( _max != 0 )
                delete[] _max;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( int* ) &value[1] )[0] = max;
            _max = value;
        }

        void update ( int  data ) {
            *_data = data;
            _update->operator () ( );
        }

        const int * getData ( ) {
            return _data;
        }


    } ;

};

#endif