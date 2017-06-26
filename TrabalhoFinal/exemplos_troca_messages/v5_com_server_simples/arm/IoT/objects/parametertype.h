#ifndef parameter_type_h__
#define parameter_type_h__

using namespace EPOS;

namespace IoT {
    
class ParameterType {
public:
    typedef unsigned char Type;
    enum {
        UNKNOWN = 0,
        INTEGER = 1,
        UNSIGNED_INTEGER = 2,
        SHORT = 3,
        UNSIGNED_SHORT = 4,
        FLOAT = 5,
        UNSIGNED_FLOAT = 6,
        CHAR = 7,
        UNSIGNED_CHAR = 8,
        BOOLEAN = 9,
        COMBO = 10
    };
protected:
    const char * _min;
    const char * _max;
    const char * _options;
public:
    ParameterType()
    : _min(0), _max(0), _options(0){}

    /* Getters */
    virtual Type getType() = 0;
    const char * getMinValue(){ return _min; }
    const char * getMaxValue(){ return _max; }
    const char * getOptions(){ return _options; };
};

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

};

#endif