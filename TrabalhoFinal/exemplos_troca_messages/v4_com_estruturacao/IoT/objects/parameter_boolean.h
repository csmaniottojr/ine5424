#ifndef parameter_boolean_h__
#define parameter_boolean_h__

using namespace EPOS;

namespace IoT {
    
class ParameterBoolean : public ParameterType {
public:
    static const unsigned char VALUE_SIZE = sizeof(bool);
protected:
public:
    ParameterBoolean()
    : ParameterType() {
        setMinValue(false);
        setMaxValue(true);
    }

    /* Getters */
    ParameterType::Type getType(){ return ParameterType::BOOLEAN; };

    /* Setters */
    void setMinValue(bool min){
        if(_min != 0)
            delete _min;

        char * value = new char[VALUE_SIZE+1];
        memset(value, '\0', VALUE_SIZE+1);

        value[0] = VALUE_SIZE;
        ((bool*) &value[1])[0] = min;
        _min = value;
    }
    void setMaxValue(bool max){
        if(_max != 0)
            delete _max;

        char * value = new char[VALUE_SIZE+1];
        memset(value, '\0', VALUE_SIZE+1);

        value[0] = VALUE_SIZE;
        ((bool*) &value[1])[0] = max;
        _max = value;
    };
};

};

#endif