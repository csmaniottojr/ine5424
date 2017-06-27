#ifndef parameter_combo_h__
#define parameter_combo_h__

#include "callback.h"
#include "parametertype.h"


using namespace EPOS;
EPOS::OStream debug;
namespace IoT {

    typedef Simple_List<char> Option_List;
    typedef List_Elements::Singly_Linked<char> Option_List_Element;

    class ParameterCombo : public ParameterType
    {
    protected:
        Option_List _options;
        int * _current_option_index;

    public:

        ParameterCombo ( )
        : ParameterType ( ), _current_option_index ( 0 ) {
            this->_update = new Callback ( );
            this->_type = COMBO;
        }

        ParameterCombo ( Callback * update , int* data )
        : ParameterType ( ) , _current_option_index ( data ) {
            this->_update = update;
            this->_type = COMBO;
        }

        Option_List * getOptionsList ( ) {
            return &_options;
        }

        const char* getOption ( int index ) {
            auto i = _options.begin ( );
            for ( ; index > 0; index-- ) {
                i++;
            }
            Option_List_Element * requested_element = i;
            return requested_element->object ( );
        }

        /* Adders */
        int addOption ( const char * option ) {
            unsigned int length = strlen ( option );
            char * copy = new char[length + 1];
            strcpy ( copy, option );
            Option_List_Element *new_option =
                    new Option_List_Element ( copy );
            _options.insert ( new_option );
            return _options.size ( ) - 1;
        }

        void update ( int index ) {
            if ( index < _options.size ( ) ) {
                *_current_option_index = index;
                _update->operator () ( );
            }
        }

        const int * getData ( ) {
            return _current_option_index;
        }

    } ;

};

#endif