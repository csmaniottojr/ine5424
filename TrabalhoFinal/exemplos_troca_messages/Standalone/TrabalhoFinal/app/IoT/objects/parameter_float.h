#ifndef parameter_float_h__
#define parameter_float_h__

using namespace EPOS;

namespace IoT {

    class ParameterFloat : public ParameterType
    {
    public:
        static const unsigned char VALUE_SIZE = sizeof (float );
    protected:

    public:

        ParameterFloat ( )
        : ParameterType ( ) { }

        ParameterFloat ( float min, float max )
        : ParameterType ( ) {
            setMinValue ( min );
            setMaxValue ( max );
        }

        /* Getters */
        ParameterType::Type getType ( ) {
            return ParameterType::FLOAT;
        };

        /* Setters */
        void setMinValue ( float min ) {
            if ( _min != 0 )
                delete _min;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( float* ) &value[1] )[0] = min;
            _min = value;
        }

        void setMaxValue ( float max ) {
            if ( _max != 0 )
                delete _max;

            char * value = new char[VALUE_SIZE + 1];
            memset ( value, '\0', VALUE_SIZE + 1 );

            value[0] = VALUE_SIZE;
            ( ( float* ) &value[1] )[0] = max;
            _max = value;
        };
    } ;

};

#endif