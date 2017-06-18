#ifndef parameter_combo_h__
#define parameter_combo_h__

using namespace EPOS;

namespace IoT {
    
class ParameterCombo : public ParameterType {
protected:
public:
    ParameterCombo()
    : ParameterType() {}

    /* Getters */
    ParameterType::Type getType(){ return ParameterType::COMBO; };
};

};

#endif