#ifndef messages_h__
#define messages_h__

#include <ieee802_15_4.h>
#include <machine.h>

using namespace EPOS;

class Message: public IEEE802_15_4::Header
{
public:

    typedef unsigned char MessageType;
    enum {
        REGISTER_REQUEST = 0,
        REGISTER_RESPONSE = 1,
        REGISTER_SERVICE_REQUEST = 2,
        REGISTER_SERVICE_RESPONSE = 3,
        REGISTER_PARAMETER_REQUEST = 4,
        REGISTER_PARAMETER_RESPONSE = 5,
        REGISTER_OPTIONS_REQUEST = 6,
        REGISTER_OPTIONS_RESPONSE = 7,
        END_REGISTER_OPTIONS_REQUEST = 8,
        END_REGISTER_OPTIONS_RESPONSE = 9,
        END_REGISTER_PARAMETER_REQUEST = 10,
        END_REGISTER_PARAMETER_RESPONSE = 11,
        END_REGISTER_SERVICE_REQUEST = 12,
        END_REGISTER_SERVICE_RESPONSE = 13,
        END_REGISTER_REQUEST = 14,
        END_REGISTER_RESPONSE = 15
    };

    static const unsigned int MTU = IEEE802_15_4::MTU - sizeof(IEEE802_15_4::Header) - sizeof(IEEE802_15_4::CRC) - sizeof(char) - sizeof(char)*4 - sizeof(MessageType);

public:
    Message() {}

    Message(MessageType type)
    : _type(type), _size(0) {
        memcpy(_id, &Machine::id()[4], 4);
    }

    Message(MessageType type, unsigned char size)
    : _type(type) {
        assert(size <= MTU);
        _size = size;
        memcpy(_id, &Machine::id()[4], 4);
    }

    Message(unsigned char id[4], MessageType type)
    : _type(type), _size(0) {
        memcpy(_id, id, 4);
    }

    //virtual bool valid() = 0;
    unsigned char * id() { return _id; }
    unsigned char size(){ return _size; }
    MessageType type(){ return _type; }

    void setId(unsigned char id[4]){
        memcpy(_id, id, 4);
    }

protected:
    unsigned char _size;
    unsigned char _id[4];
    MessageType _type;
    IEEE802_15_4::CRC _crc;
} __attribute__((packed));//==> End Message

class RegisterRequest : public Message
{
public:
    RegisterRequest() 
    : Message(Message::REGISTER_REQUEST) {}

    bool valid(){ return _type == Message::REGISTER_REQUEST; }

private:

} __attribute__((packed));//==> End RegisterRequest

class RegisterResponse : public Message
{
public:
    RegisterResponse()
    : Message(Message::REGISTER_RESPONSE), _sendData(0) {}

    RegisterResponse(bool alreadyRegistered)
    : Message(Message::REGISTER_RESPONSE), _sendData(!alreadyRegistered) {}
    
    RegisterResponse(unsigned char id[4]) 
    : Message(id, Message::REGISTER_RESPONSE) {}

    bool valid(){ return _type == Message::REGISTER_RESPONSE; }
    bool sendData(){ return _sendData; }

    void setSendData(bool value){ _sendData = value; }

private:
    unsigned char _sendData;
} __attribute__((packed));//==> End RegisterResponse

class RegisterServiceRequest : public Message
{
public:
    RegisterServiceRequest(const char * service_name)  
    : Message(Message::REGISTER_SERVICE_REQUEST, strlen(service_name)) {
        strcpy(_service_name, service_name);
    }

    bool valid(){ return _type == Message::REGISTER_SERVICE_REQUEST; }
    const char * serviceName(){ return _service_name; }

private:
    char _service_name[50];
} __attribute__((packed));//==> End RegisterServiceRequest

class RegisterServiceResponse : public Message
{
public:
    RegisterServiceResponse()
    : Message(Message::REGISTER_SERVICE_RESPONSE) {}

    RegisterServiceResponse(unsigned char id[4]) 
    : Message(id, Message::REGISTER_SERVICE_RESPONSE) {}

    bool valid(){ return _type == Message::REGISTER_SERVICE_RESPONSE; }

private:
    
} __attribute__((packed));//==> End RegisterServiceResponse

#endif
