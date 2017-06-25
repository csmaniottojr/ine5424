#ifndef parameter_combo_h__
#define parameter_combo_h__

using namespace EPOS;

namespace IoT {
    
    typedef Simple_List<char> Option_List;
    typedef List_Elements::Singly_Linked<char> Option_List_Element;

class ParameterCombo : public ParameterType {
protected:
    Option_List _options;
public:
    ParameterCombo()
    : ParameterType() {}

    /* Getters */
    ParameterType::Type getType(){ return ParameterType::COMBO; };
    Option_List * getOptionsList(){ return &_options; }

    /* Adders */
    void addOption(const char * option){
        unsigned int length = strlen(option);
        char * copy = new char[length+1];
        memset(copy, '\0', length+1);
        memcpy(copy, option, length);

        Option_List_Element *new_option = 
            new Option_List_Element(copy);
        _options.insert(new_option);
    }
};

};

#endif