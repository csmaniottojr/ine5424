#ifndef OBJECTS_H
#define OBJECTS_H
#include <utility/list.h>
#include <utility/string.h>

//REMOVE THIS AFTER DEBUG FOR GOD SAKE!
#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define DEBUG(x) debug << x << "\n"
#else
#define DEBUG(x) do {} while (0)
#endif
__BEGIN_SYS
//==============================================================================
//      Types   
//Forward declaration to type definitions
class Parameter;
class Service;
EPOS::OStream debug;


typedef char* string;

typedef Simple_List<Parameter> Parameter_List;
typedef List_Elements::Singly_Linked<Parameter> Parameter_List_Element;

typedef Simple_List<Service> Service_List;
typedef List_Elements::Singly_Linked<Service> Service_List_Element;

typedef Simple_List<char> String_List;
typedef List_Elements::Singly_Linked<char> String_List_Element;


const string EMPTY_STRING = ( char* ) "";
//Classes here!=================================================================

class Smart_Object
{
public:

    Smart_Object ( string name );

    void insert_service ( Service & new_service ) {
        Service_List_Element * new_service_element =
                new Service_List_Element ( &new_service );
        this->_services.insert ( new_service_element );
    }

    virtual ~Smart_Object ( ) {/*Delete all services in Service_list*/ }

private:
    string _name;
    Service_List _services;
};
//==============================================================================

class Service
{
public:

    Service ( string name ) : _name ( name ) { }

    void insert_parameter ( Parameter & new_parameter ) {
        Parameter_List_Element * new_parameter_element =
                new Parameter_List_Element ( &new_parameter );
        this->_parameters.insert ( new_parameter_element );
    }

    string name ( ) {
        return _name;
    }
private:
    string _name;
    Parameter_List _parameters;
};
//==============================================================================

class Parameter
{
public:
    //Possible parameter types

    enum class Param_Type
    {
        DOUBLE,
        BOOLEAN,
        COMBO
    };
    //Constructor

    Parameter ( string name, Param_Type type )
    : _name ( name ), _type ( type ) { }

    //Get Parameter type

    Param_Type type ( ) {
        return _type;
    };

    //get name

    string name ( ) {
        return _name;
    }

    //Get combo size

    int combo_counter ( ) {
        return _combo_counter;
    }

    //Remember to memcpy!
    //Insert a new combo parameter

    int insert_combo ( string combo ) {
        //Must increment combo counter, memcpy
        int combo_size = strlen ( combo );
        char* copy = new char[combo_size];

        strcpy ( copy, combo );
        String_List_Element * new_combo =
                new String_List_Element ( copy );
        this->_combo_values.insert ( new_combo );
        this->_combo_counter = _combo_values.size ( );
        return _combo_counter - 1;

    }

    //Get bool format data

    bool bool_data ( ) {
        return _bool_data;
    }

    void bool_data ( bool data ) {
        _bool_data = data;
    }

    //Get double format data

    double double_data ( ) {
        return _double_data;
    }

    //Get combo_data[combo_number]

    string combo ( int combo_number ) {
        String_List_Element * x = this->_combo_values.head ( );
        for ( unsigned char i = 0; ( int ) i < combo_number; ( int ) i++ ) {
            x = x->next ( );
        }
        return ( x->object ( ) );
    }
private:

    union
    {
        bool _bool_data;
        double _double_data;
        int _combo_index; //Index of the actual combo value
    };

    string _name; //Parameter Name
    int _id; //Parameter Id
    unsigned int _combo_counter; //Number of combo elements
    Param_Type _type; //Parameter type
    double _min; //Minimum for double parameters
    double _max; //Maximum for double parameters
    String_List _combo_values; //combo_values


};
#endif
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================
__END_SYS