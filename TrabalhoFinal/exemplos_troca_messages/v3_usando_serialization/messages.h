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
        UNKNOWN_MESSAGE                 = 0,
        REGISTER_REQUEST                = 1,
        REGISTER_RESPONSE               = 2,
        REGISTER_SERVICE_REQUEST        = 3,
        REGISTER_SERVICE_RESPONSE       = 4,
        REGISTER_INT_PARAM_REQUEST      = 5,
        REGISTER_FLOAT_PARAM_REQUEST    = 6,
        REGISTER_PARAM_RESPONSE         = 7
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
        setSizeAddedWithBaseSize(messageSize);
        memcpy(_id, &Machine::id()[ID_SIZE], ID_SIZE);
    }

    Size size(){ return _size; }
    void setSize(Size size){ _size = size; }
    void setSizeAddedWithBaseSize(Size value){ _size = BASE_SIZE + value; }
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
        setSizeAddedWithBaseSize(size);
    }
};

class RegisterServiceResponse : public RegisterMessage {
public:
    RegisterServiceResponse()
    : RegisterMessage(RegisterMessage::REGISTER_SERVICE_RESPONSE) {}
};

class RegisterIntParameterRequest : public RegisterMessage {
public:
    static const unsigned char BASE_SIZE = sizeof(int)*2;

protected:
    char * _param_name;
    int _min;
    int _max;
public:
    RegisterIntParameterRequest()
    : RegisterMessage(BASE_SIZE, RegisterMessage::REGISTER_INT_PARAM_REQUEST),
      _param_name(0) {}

    RegisterIntParameterRequest(const char * param_name, int min, int max)
    : RegisterMessage(RegisterMessage::REGISTER_INT_PARAM_REQUEST),
      _param_name(0), _min(min), _max(max) {
        setParamName(param_name, strlen(param_name));
    }

    int min(){ return _min; }
    void setMin(int value){ _min = value; }
    int max(){ return _max; }
    void setMax(int value){ _max = value; }

    const char * paramName(){ return _param_name; }
    void setParamName(const char * param_name, Size size){
        //TODO provavelmente tem jeito melhor de fazer tudo isso... ;x
        if(_param_name != 0)
            delete[] _param_name;

        _param_name = new char[size+1];
        memset(_param_name, '\0', size+1);
        strncpy(_param_name, param_name, size);
        setSizeAddedWithBaseSize(BASE_SIZE + size);
    }
};

class RegisterFloatParameterRequest : public RegisterMessage {
public:
    static const unsigned char BASE_SIZE = sizeof(float)*2;

protected:
    char * _param_name;
    float _min;
    float _max;
public:
    RegisterFloatParameterRequest()
    : RegisterMessage(BASE_SIZE, RegisterMessage::REGISTER_FLOAT_PARAM_REQUEST),
      _param_name(0) {}

    RegisterFloatParameterRequest(const char * param_name, float min, float max)
    : RegisterMessage(RegisterMessage::REGISTER_FLOAT_PARAM_REQUEST),
      _param_name(0), _min(min), _max(max) {
        setParamName(param_name, strlen(param_name));
    }

    float min(){ return _min; }
    void setMin(float value){ _min = value; }
    float max(){ return _max; }
    void setMax(float value){ _max = value; }

    const char * paramName(){ return _param_name; }
    void setParamName(const char * param_name, Size size){
        //TODO provavelmente tem jeito melhor de fazer tudo isso... ;x
        if(_param_name != 0)
            delete[] _param_name;

        _param_name = new char[size+1];
        memset(_param_name, '\0', size+1);
        strncpy(_param_name, param_name, size);
        setSizeAddedWithBaseSize(BASE_SIZE + size);
    }
};

class RegisterParamResponse : public RegisterMessage {
public:
    RegisterParamResponse()
    : RegisterMessage(RegisterMessage::REGISTER_PARAM_RESPONSE) {}
};

#endif