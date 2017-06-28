#ifndef parameter_float_h__
#define parameter_float_h__

#include "callback.h"
#include "parametertype.h"


using namespace EPOS;

namespace IoT {

    class ParameterFloat : public ParameterType
    {
    public:
        static const unsigned char VALUE_SIZE = sizeof (float );
    private:
        float * _data;

    public:

        ParameterFloat ( )
        : ParameterType ( ), _data ( 0 ) {
            this->_update = new Callback ( ); //Does nothing!
            this->_type = FLOAT;
        }

        ParameterFloat ( Callback * update ,  float * data, float min_, float max_ )
        : ParameterType ( ) , _data ( data ) {
            setMinValue ( min_ );
            setMaxValue ( max_ );
            this-> _update  = ( update );
            this->_type = FLOAT;
        }

        /* Setters */
        void setMinValue ( float min ) {
            if ( _min != 0 )
                delete[] _min;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( float* ) &value[1] )[0] = min;
            _min = value;
        }

        void setMaxValue ( float max ) {
            if ( _max != 0 )
                delete[] _max;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( float* ) &value[1] )[0] = max;
            _max = value;
        }

        void update ( float  data ) {
            *_data = data;
            _update->operator () ( );
        }

        const float * getData ( ) {
            return _data;
        }


    } ;

};

#endif