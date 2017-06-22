#ifndef smartobject_h__
#define smartobject_h__

#include "service.h"
#include "parameter.h"

using namespace EPOS;

namespace IoT {

    typedef Simple_List<Service> Service_List;
    typedef List_Elements::Singly_Linked<Service> Service_List_Element;

    class Smart_Object
    {
    public:
        static const unsigned char ID_SIZE = 4;
        typedef unsigned char ID[ID_SIZE];
    protected:
        ID _id;
        const char * _name;
        Service_List _services;
    public:

        Smart_Object ( const char * name )
        : _name ( name ) {
            memcpy ( _id, &Machine::id ( )[ID_SIZE], ID_SIZE );
        }

        /* Getters */
        const unsigned char * id ( ) {
            return _id;
        }

        const char * name ( ) {
            return _name;
        }

        Service_List * services ( ) {
            return &_services;
        }

        /* Setters */
        void name ( const char * name ) {
            _name = name;
        }

        void id ( const unsigned char * id ) {
            memcpy ( _id, id, ID_SIZE );
        }

        /* 'Adders' */
        void add_service ( Service *service ) {
            Service_List_Element *new_service =
                    new Service_List_Element ( service );
            _services.insert ( new_service );
        }
    } ;

};

#endif