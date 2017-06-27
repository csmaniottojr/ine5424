#ifndef smartobject_h__
#define smartobject_h__

#include "service.h"
#include "parameter.h"

using namespace EPOS;

namespace IoT {

    typedef Simple_List<Service> Service_List;
    typedef List_Elements::Singly_Linked<Service> Service_List_Element;

    class SmartObject
    {
    public:
        typedef unsigned int ID;
    protected:
        ID _id;
        const char * _name;
        Service_List _services;
    public:

        SmartObject ( const char * name )
        : _name ( name ) {
            _id = *( ( ID* ) & Machine::id ( )[4] );
        }

        /* Getters */
        ID getId ( ) {
            return _id;
        }

        const char * getName ( ) {
            return _name;
        }

        Service_List * getServices ( ) {
            return &_services;
        }

        Parameter* getById ( Parameter::RegisterIdValue id ) {
            for ( auto i = _services.head ( ); i; i = i->next ( ) ) {
                auto parameter = i->object ( )->findById ( id );
                if ( parameter != 0 ) {
                    return parameter;
                }
            }
            return 0;
        }

        /* Setters */
        void setName ( const char * name ) {
            _name = name;
        }

        void setId ( ID id ) {
            _id = id;
        }

        /* 'Adders' */
        void addService ( Service * service ) {
            Service_List_Element *new_service =
                    new Service_List_Element ( service );
            _services.insert ( new_service );
        }
    } ;

} ;

#endif