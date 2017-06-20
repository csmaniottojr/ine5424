#ifndef parameter_h__
#define parameter_h__

#include "parametertype.h"

using namespace EPOS;

namespace IoT {
    
class Parameter {
protected:
    const char * _name;
    unsigned short _registerId;
    ParameterType *_type;
public:
    Parameter(const char * name, unsigned short registerId, ParameterType * type)
    : _name(name), _registerId(registerId), _type(type) {}

    /* Getters */
    const char * getName(){ return _name; }
    unsigned short getRegisterId(){ return _registerId; }
    ParameterType::Type getType(){ return _type->getType(); }
    const char * getMinValue(){ return _type->getMinValue(); }
    const char * getMaxValue(){ return _type->getMaxValue(); }
    const char * getOptions(){ return _type->getOptions(); }
    ParameterType * getParameterType(){ return _type; }
};

};

#endif