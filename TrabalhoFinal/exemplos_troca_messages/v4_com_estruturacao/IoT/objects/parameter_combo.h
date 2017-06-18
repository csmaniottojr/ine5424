#ifndef parameter_combo_h__
#define parameter_combo_h__

using namespace EPOS;

namespace IoT {
    
class ParameterCombo : public ParameterType {
protected:
public:
    ParameterCombo(const char * options)
    : ParameterType() {
        _options = options;
    }

    /* Getters */
    ParameterType::Type getType(){ return ParameterType::COMBO; };
};

};

#endif