#ifndef parameter_type_h__
#define parameter_type_h__

#include "callback.h"
#include "parameter.h"



using namespace EPOS;

namespace IoT {

    class Parameter_Type
    {
    public:
        typedef unsigned char Type;

        enum
        {
            NONE =              0,
            FLOAT =             1,
            BOOLEAN =           2,
            COMBO =             3
        } ;
    protected:
        const char * _min;
        const char * _max;
        /*Update function is called everytime the parameter is updated!*/
        Callback * _update; //Is a pointer already!
        Type _type;

    public:

        Parameter_Type ( )
        : _min ( 0 ), _max ( 0 ) {
            _type = NONE;
        }

        /* Getters */
        virtual Type type ( ) {
            return _type;
        };

        const char * min ( ) {
            return _min;
        }

        const char * max ( ) {
            return _max;
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