#ifndef parameter_float_h__
#define parameter_float_h__

#include "callback.h"
#include "parametertype.h"


using namespace EPOS;

namespace IoT {

    class Parameter_Float : public Parameter_Type
    {
    public:
        static const unsigned char VALUE_SIZE = sizeof (float );
    private:
        float * _data;

    public:

        Parameter_Float ( )
        : Parameter_Type ( ) {
            this->_type = FLOAT;
            this->_update = new Callback ( ); //Does nothing!
        }

        Parameter_Float ( Callback * update ,  float * data, float max_, float min_ )
        : Parameter_Type ( ) , _data ( data ) {
            min ( min_ );
            max ( max_ );
            this-> _update  = ( update );
            this->_type = FLOAT;
        }

        /* Setters */
        void min ( float min ) {
            if ( _min != 0 )
                delete _min;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( float* ) &value[1] )[0] = min;
            _min = value;
        }

        void max ( float max ) {
            if ( _max != 0 )
                delete _max;

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

    } ;

};

#endif