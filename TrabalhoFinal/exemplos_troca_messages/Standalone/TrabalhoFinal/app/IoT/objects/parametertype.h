#ifndef parameter_type_h__
#define parameter_type_h__

#include "callback.h"
#include "parameter.h"



using namespace EPOS;

namespace IoT {

    class ParameterType
    {
    public:
        typedef unsigned char Type;
        enum
        {
            NONE =              0,
            FLOAT =             1,
            BOOLEAN =           2,
            COMBO =             3,
            INTEGER =           4
        } ;
    protected:
        const char * _min;
        const char * _max;
        /*Update function is called everytime the parameter is updated!*/
        Callback * _update; //Is a pointer already!
        Type _type;

    public:

        ParameterType ( )
        : _min ( 0 ), _max ( 0 ), _type(NONE) {}

        /* Getters */
        virtual Type getType ( ) {
            return _type;
        };

        /**
         * Retorna o valor minimo do parametro.
         * O retorno é da forma: 
         * Byte 0      1     x
         *      | size | min |
         * aonde size se refere ao tamanho do valor min
         */
        const char * getMinValue ( ) {
            return _min;
        }

        /**
         * Retorna o valor máximo do parametro.
         * O retorno é da forma: 
         * Byte 0      1     x
         *      | size | max |
         * aonde size se refere ao tamanho do valor max
         */
        const char * getMaxValue ( ) {
            return _max;
        }

        Callback* update ( ) {
            return _update;
        }

    } ;

} ;
#endif
