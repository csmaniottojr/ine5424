#ifndef parameter_h__
#define parameter_h__

#include "parametertype.h"
#include "parameter_boolean.h"
#include "parameter_combo.h"
#include "parameter_float.h"

using namespace EPOS;

namespace IoT {

    class Parameter
    {
    protected:
        const char * _name;
        unsigned short _register_id;
        Parameter_Type *_type;
    public:

        Parameter ( const char * name, unsigned short registerId, Parameter_Type * type )
        : _name ( name ), _register_id ( registerId ), _type ( type ) { }

        /* Getters */
        const char * name ( ) {
            return _name;
        }

        unsigned short register_id ( ) {
            return _register_id;
        }

        Parameter_Type::Type getType ( ) {
            return _type->type ( );
        }

        Parameter_Type * getParameterType ( ) {
            return _type;
        }

        void update ( bool value ) {
            if ( _type->type ( ) == Parameter_Type::BOOLEAN ) {
                static_cast < Parameter_Boolean * > ( _type )->update ( value );
            }
        }

        void update ( float value ) {
            if ( _type->type ( ) == Parameter_Type::FLOAT ) {
                static_cast < Parameter_Float * > ( _type )->update ( value );
            }
        }

        void update ( int value ) {
            if ( _type->type ( ) == Parameter_Type::COMBO ) {
                static_cast < Parameter_Combo * > ( _type )->update ( value );
            }
        }

        int insert_combo ( char * value ) {
            if ( getType ( )  == Parameter_Type::COMBO ) {
                return static_cast < Parameter_Combo* > ( _type )->add_option ( value );
            }
            return -1; //Not a COMBO!
        }

        char * get_combo ( int index ) {
            if ( getType ( ) == Parameter_Type::COMBO ) {
                return (char*)static_cast < Parameter_Combo* > ( _type )->get_option ( index );
            }
            return 0;
        }

    } ;

};

#endif