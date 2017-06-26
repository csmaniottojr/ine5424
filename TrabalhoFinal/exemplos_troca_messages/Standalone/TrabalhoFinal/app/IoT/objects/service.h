#ifndef service_h__
#define service_h__

#include "parameter.h"

using namespace EPOS;

namespace IoT {

    typedef Simple_List<Parameter> Parameter_List;
    typedef List_Elements::Singly_Linked<Parameter> Parameter_List_Element;

    class Service
    {
    protected:
        const char * _name;
        Parameter_List _parameters;

       
    public:

        Service ( const char * name)
        : _name ( name ) { }

        /* Getters */
        const char * getName ( ) {
            return _name;
        }

        Parameter_List * getParameters ( ) {
            return &_parameters;
        }

        /* Setters */
        void setName ( const char * name ) {
            _name = name;
        }

        /* 'Adders' */
        void addParameter ( Parameter * param ) {
            Parameter_List_Element *new_param =
                    new Parameter_List_Element ( param );
            _parameters.insert ( new_param );
        }

        
    } ;

};

#endif