/* 
 * File:   IoTManager.h
 * Author: decker
 *
 * Created on June 3, 2017, 6:59 PM
 */


#ifndef IOTMANAGER_H
#define IOTMANAGER_H

#include "system/config.h"
#include "utility/list.h"
/*
 * This is a IoT metadata manager.
 * It is responsible of the creation of a IoThing descriptor that will be used 
 * to the registration of a IoThing in a IoT network. 
 * */
__BEGIN_SYS
        namespace IoT {
    typedef Simple_List<IoT_Service> IoTList;
    typedef List_Elements::Singly_Linked<IoT_Service> ListElement;

    enum data_type
    {
        INTEGER = 1,
        UNSIGNED_INTEGER = 2,
        SHORT = 3,
        UNSIGNED_SHORT = 4,
        FLOAT = 5,
        UNSIGNED_FLOAT = 6,
        UNSIGNED_CHAR = 7,
        CHAR = 8
    };

    class IoT_Manager
    {
    private:
        //List of avaible IoT Elements (functions and sensoring)
        IoTList _elements;
    public:

        IoT_Manager () { }

        void addElement (IoT_Service & newElement);
        void removeElement (IoT_Service & removalElement);


    };
};
__END_SYS
#endif /* IOTMANAGER_H */

//==============================================================================


#ifndef IOTSERVICE_H
#define IOTSERVICE_H
/*
 * This is a IOT Service.
 * A service is a action made by the smart object. This action can be:
 *      Sensoring -> Display a data obtained from the smart object
 *      Acting -> Read the parameters and actuate 
 * A service has parameters, and this parameters are considered in the service
 * execution.
 */
__BEGIN_SYS
        namespace IoT {


    typedef List_Elements::Singly_Linked<IoT_Service*> ListElement;
    typedef Simple_List<ListElement> IoTList;

    class IoT_Service
    {
    public:

        IoT_Service () { }

    protected:

        char * _description; //Element description (to be sent to IoT server)
    };
}

__END_SYS
#endif//IOTSERVICE_H
//==============================================================================
#ifndef IOTPARAMETER_H
#define IOTPARAMETER_H
/*
 * This is a IoT Parameter. 
 * A Parameter is a configuration information used in a IoT_Service.
 */
__BEGIN_SYS
        namespace IoT {

    class IoT_Parameter
    {
    private:
        unsigned char _id;
        char* _nome;
        data_type _data_type;
    };
};
__END_SYS
#endif //IOTPARAMETER_H

//==============================================================================
