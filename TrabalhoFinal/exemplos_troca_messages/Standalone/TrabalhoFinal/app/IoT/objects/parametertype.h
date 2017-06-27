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
        const char * _options;
        /*Update function is called everytime the parameter is updated!*/
        Callback * _update; //Is a pointer already!
        Type _type;

    public:

        ParameterType ( )
        : _min ( 0 ), _max ( 0 ), _options ( 0 ) {
            _type = NONE;
        }

        /* Getters */
        virtual Type getType ( ) {
            return _type;
        };

        const char * getMinValue ( ) {
            return _min;
        }

        const char * getMaxValue ( ) {
            return _max;
        }

        const char * getOptions ( ) {
            return _options;
        }

        Callback* update ( ) {
            return _update;
        }

    } ;

} ;

/* Exemplo de leitura do min e max [byte 0 tem o tamanho do valor]
    cout << "\nFloat values: " << endl;
    cout << temp.getType() << endl;

    const char * temp_min = temp.getMinValue();
    cout << "Size: " << (unsigned char) temp_min[0] << endl;

    for(int i = 1; i<=((unsigned char)temp_min[0]); i++)
        cout << (unsigned char) temp_min[i] << " ";
    cout << dec << endl;
    cout << "Min: " << ((float*) &temp_min[1])[0] << endl;

    const char * temp_max = temp.getMaxValue();
    cout << "Size: " << (unsigned char) temp_max[0] << endl;

    for(int i = 1; i<=((unsigned char)temp_max[0]); i++)
        cout << (unsigned char) temp_max[i] << " ";
    cout << dec << endl;
    cout << "Max: " << ((float*) &temp_max[1])[0] << endl;
 */



#endif
