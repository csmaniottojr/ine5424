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
    public:
        typedef unsigned short RegisterIdValue;
    protected:
        const char * _name;
        RegisterIdValue _register_id;
        bool _read_only;
        ParameterType *_type;

        union
        {
            int _index;
            float _value;
            bool _flag;
        } ;

    public:

        Parameter ( const char * name, unsigned short registerId, ParameterType * type )
        : _name ( name ), _register_id ( registerId ), _type ( type ),
          _read_only ( false ) { }
        
        Parameter ( const char * name, unsigned short registerId, 
            bool read_only, ParameterType * type )
        : _name ( name ), _register_id ( registerId ), _type ( type ),
          _read_only( read_only ) { }

        /* Getters */
        const char * getName ( ) {
            return _name;
        }

        unsigned short getRegisterId ( ) {
            return _register_id;
        }

        bool isReadOnly(){
            return _read_only;
        }

        void setReadOnly(bool value){
            _read_only = value;
        }

        ParameterType::Type getType ( ) {
            return _type->getType ( );
        }

        ParameterType * getParameterType ( ) {
            return _type;
        }

        const char * getMinValue ( ) {
            return _type->getMinValue ( );
        }

        const char * getMaxValue ( ) {
            return _type->getMaxValue ( );
        }

        void update ( bool value ) {
            if ( _type->getType ( ) == ParameterType::BOOLEAN ) {
                static_cast < ParameterBoolean * > ( _type )->update ( value );
                _flag = value;
            }
        }

        void update ( float value ) {
            if ( _type->getType ( ) == ParameterType::FLOAT ) {
                static_cast < ParameterFloat * > ( _type )->update ( value );
                float max, min;
                max = *( float* ) ( _type->getMaxValue ( ) + 1 );
                min = *( float* ) ( _type->getMinValue ( ) + 1 );

                if ( value <= max && value >= min ) {
                    _value = value;
                }
            }
        }

        void update ( int value ) {
            if ( _type->getType ( ) == ParameterType::COMBO ) {
                static_cast < ParameterCombo * > ( _type )->update ( value );
                _index = value;
            }
        }

        int insertCombo ( char * value ) {
            if ( getType ( )  == ParameterType::COMBO ) {
                return static_cast < ParameterCombo* > ( _type )->addOption ( value );
            }
            return -1; //Not a COMBO!
        }

        char * getCombo ( int index ) {
            if ( getType ( ) == ParameterType::COMBO ) {
                return ( char* ) static_cast < ParameterCombo* > ( _type )->getOption ( index );
            }
            return 0;
        }

        int comboValue ( ) {
            if ( getType ( ) == ParameterType::COMBO ) {
                _type->update ( )->operator () ( );
                _index =  * ( static_cast < ParameterCombo* > ( _type )->getData ( ) ) ;
                return _index;
            }
        }

        float floatValue ( ) {
            if ( getType ( ) == ParameterType::FLOAT ) {
                _type->update ( )->operator () ( );
                _value = * ( static_cast < ParameterFloat* > ( _type )->getData ( ) ) ;
                return _value;
            }
        }

        bool boolValue ( ) {
            if ( getType ( ) == ParameterType::BOOLEAN ) {
                _type->update ( )->operator () ( );
                _flag =  * ( static_cast < ParameterBoolean* > ( _type )->getData ( ) );
                return _flag;
            }
        }


    } ;

} ;

#endif