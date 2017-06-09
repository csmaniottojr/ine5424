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
    : _size(0), _type(UNKNOWN_MESSAGE){
        memcpy(_id, &Machine::id()[ID_SIZE-1], ID_SIZE);
    }

    RegisterMessage(Type type)
    : _size(0), _type(type){
        memcpy(_id, &Machine::id()[ID_SIZE-1], ID_SIZE);
    }
    
    RegisterMessage(Size messageSize, Type type)
    : _size(messageSize+BASE_SIZE), _type(type){
        memcpy(_id, &Machine::id()[ID_SIZE-1], ID_SIZE);
    }

    Size size(){ return _size; }
    void setSize(Size size){ _size = size; }
    unsigned char * id(){ return _id; }
    void setId(ID id){ memcpy(_id, id, ID_SIZE); }
    Type type(){ return _type; }
    void setType(Type type){ _type = type; }

    void parseMessage(const char * msg){
        _size = (unsigned char) msg[0];
        memcpy(_id, &msg[1], ID_SIZE);
        _type = (unsigned char)msg[ID_SIZE+1];
    }

    const char * toString(){
        int length = BASE_SIZE;
        char * result = new char[length];
        memset(result, '\0', length);

        result[0] = _size;
        memcpy(&result[1], _id, ID_SIZE);
        result[ID_SIZE+1] = _type;
        return result;
    }
};

class RegisterServiceRequest : public RegisterMessage {
protected: 
    char * _service_name;
public:
    RegisterServiceRequest()
    : RegisterMessage(RegisterMessage::REGISTER_SERVICE_REQUEST),
      _service_name(0) {}

    RegisterServiceRequest(const char * service_name)
    : RegisterMessage(strlen(service_name), RegisterMessage::REGISTER_SERVICE_REQUEST),
      _service_name(0) {
        setServiceName(service_name, strlen(service_name));
    }

    const char * serviceName(){ return _service_name; }
    void setServiceName(const char * service_name, unsigned char size){
        //TODO provavelmente tem jeito melhor de fazer tudo isso... ;x
        if(_service_name != 0)
            delete[] _service_name;

        _service_name = new char[size+1];
        memset(_service_name, '\0', size+1);
        strncpy(_service_name, service_name, size);
    }

    void parseMessage(const char * msg){
        RegisterMessage::parseMessage(msg);
        if(_type == RegisterMessage::REGISTER_SERVICE_REQUEST){
            unsigned char length = (_size - RegisterMessage::BASE_SIZE);
            setServiceName(&msg[RegisterMessage::BASE_SIZE], length);
        }
    }

    const char * toString(){
        int length = _size;
        char * result = new char[length];
        memset(result, '\0', length);

        const char * base = RegisterMessage::toString();
        int l = RegisterMessage::BASE_SIZE;
        memcpy(result, base, l);
        delete[] base;

        strncpy(&result[l], _service_name, strlen(_service_name));
        return result;
    }
};

#endif