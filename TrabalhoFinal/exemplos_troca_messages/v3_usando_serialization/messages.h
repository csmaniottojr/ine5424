#ifndef messages_h__
#define messages_h__

#include <machine.h>

using namespace EPOS;

//TODO verificar os sizes
class RegisterMessage {
public:
    static const unsigned char ID_SIZE = 4;

    typedef unsigned char Size;
    typedef unsigned char ID[ID_SIZE];
    typedef unsigned char Type;
    enum {
        UNKNOWN_MESSAGE             = 0,
        REGISTER_REQUEST            = 1,
        REGISTER_RESPONSE           = 2,
        REGISTER_SERVICE_REQUEST    = 3,
        REGISTER_SERVICE_RESPONSE   = 4
    };

    static const unsigned char BASE_SIZE = sizeof(Size) + sizeof(ID) + sizeof(Type);

protected:
    Size _size;
    ID _id;
    Type _type;

public:
    RegisterMessage()
    : _size(BASE_SIZE), _type(UNKNOWN_MESSAGE){
        memcpy(_id, &Machine::id()[ID_SIZE], ID_SIZE);
    }

    RegisterMessage(Type type)
    : _size(BASE_SIZE), _type(type){
        memcpy(_id, &Machine::id()[ID_SIZE], ID_SIZE);
    }
    
    RegisterMessage(Size messageSize, Type type)
    : _size(BASE_SIZE), _type(type){
        addToSize(messageSize);
        memcpy(_id, &Machine::id()[ID_SIZE], ID_SIZE);
    }

    Size size(){ return _size; }
    void setSize(Size size){ _size = size; }
    void addToSize(Size value){ _size += value; }
    const unsigned char * id(){ return _id; }
    void setId(const unsigned char * id){ memcpy(_id, id, ID_SIZE); }
    Type type(){ return _type; }
    void setType(Type type){ _type = type; }
};

class RegisterRequest : public RegisterMessage {
public:
    RegisterRequest()
    : RegisterMessage(RegisterMessage::REGISTER_REQUEST) {}
};

class RegisterResponse : public RegisterMessage {
protected:
    unsigned char _send_data;
public:
    RegisterResponse()
    : RegisterMessage(sizeof(unsigned char), RegisterMessage::REGISTER_RESPONSE) {}

    bool sendData(){ return _send_data; }
    void setSendData(bool value){ _send_data = value; }
};

class RegisterServiceRequest : public RegisterMessage {
protected: 
    char * _service_name;
public:
    RegisterServiceRequest()
    : RegisterMessage(RegisterMessage::REGISTER_SERVICE_REQUEST),
      _service_name(0) {}

    RegisterServiceRequest(const char * service_name)
    : RegisterMessage(RegisterMessage::REGISTER_SERVICE_REQUEST),
      _service_name(0) {
        setServiceName(service_name, strlen(service_name));
    }

    const char * serviceName(){ return _service_name; }
    void setServiceName(const char * service_name, Size size){
        //TODO provavelmente tem jeito melhor de fazer tudo isso... ;x
        if(_service_name != 0)
            delete[] _service_name;

        _service_name = new char[size+1];
        memset(_service_name, '\0', size+1);
        strncpy(_service_name, service_name, size);
        addToSize(size);
    }
};

class RegisterServiceResponse : public RegisterMessage {
public:
    RegisterServiceResponse()
    : RegisterMessage(RegisterMessage::REGISTER_SERVICE_RESPONSE) {}
};

#endif