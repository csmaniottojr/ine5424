#ifndef parameter_combo_h__
#define parameter_combo_h__

#include "callback.h"
#include "parametertype.h"


using namespace EPOS;
EPOS::OStream debug;
namespace IoT {

    typedef Simple_List<char> Option_List;
    typedef List_Elements::Singly_Linked<char> Option_List_Element;

    class Parameter_Combo : public Parameter_Type
    {
    protected:
        Option_List _options;
        int * _current_option_index;
        //        int & _data = _current_option_index;
    public:

        Parameter_Combo ( Callback * update , int* data )
        : Parameter_Type ( ) , _current_option_index ( data ) {
            this->_update = update;
            this->_type = COMBO;
        }

        Parameter_Combo ( )
        : Parameter_Type ( ) {
            this->_update = new Callback ( );
            this->_type = COMBO;
        }

        Option_List * get_options_list ( ) {
            return &_options;
        }

        const char* get_option ( int index ) {
            auto i = _options.begin ( );
            for ( ; index > 0; index-- ) {
                i++;
            }
            Option_List_Element * requested_element = i;
            return requested_element->object ( );
        }

        /* Adders */
        int add_option ( const char * option ) {
            unsigned int length = strlen ( option );
            char * copy = new char[length + 1];
            strcpy ( copy, option );
            Option_List_Element *new_option =
                    new Option_List_Element ( copy );
            _options.insert ( new_option );
            return _options.size ( ) - 1;
        }

        void update ( int index ) {
            debug << "update combo called " << index << " " << _options.size ( ) - 1 << "\n" ;
            if ( index < _options.size ( ) ) {
                *_current_option_index = index;
                _update->operator () ( );
            }
        }

        const int * data ( ) {
            return _current_option_index;
        }

    } ;

};

#endif