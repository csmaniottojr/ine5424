#ifndef command_message_h__
#define command_message_h__

using namespace EPOS;

namespace IoT {

class CommandMessage {
public:
    typedef unsigned char Size;
    typedef SmartObject::ID ID;
    typedef unsigned char Type;
    enum {
        UNKNOWN_MESSAGE                 = 0,
        FIRST_TYPE                      = 1,

        COMMAND_READ_REQUEST            = FIRST_TYPE,
        COMMAND_READ_RESPONSE           = 2,
        COMMAND_WRITE_REQUEST           = 3,
        COMMAND_WRITE_RESPONSE          = 4,

        LAST_TYPE                       = COMMAND_WRITE_RESPONSE
    };
    typedef Parameter::RegisterIdValue RegisterIdValue;
    typedef unsigned char Data;

    static const char START_CHAR = ':';
    static const Size BASE_SIZE = sizeof(START_CHAR) + sizeof(Size) + sizeof(ID)
                                + sizeof(Type) + sizeof(RegisterIdValue);
    static const Size MTU = IEEE802_15_4::Frame::MTU - BASE_SIZE;
protected:
    Size _size;
    ID _id;
    Type _type;
    RegisterIdValue _registerId;
    const Data * _data;
public:
    CommandMessage()
    : _size(BASE_SIZE), _type(UNKNOWN_MESSAGE){
        _id = *((ID*) &Machine::id()[4]);
    }

    CommandMessage(Type type)
    : _size(BASE_SIZE), _type(type){
        _id = *((ID*) &Machine::id()[4]);
    }
    
    CommandMessage(RegisterIdValue regId, const Data * data, Size dataLength, Type type)
    : _size(BASE_SIZE+dataLength), _type(type), _registerId(regId) {
        assert(_size <= MTU);
        _id = *((ID*) &Machine::id()[4]);
        _data = data;
    }

    Size getSize(){ return _size; }
    void setSize(Size size){ 
        _size = size;
        assert(_size >= BASE_SIZE && _size <= MTU); 
    }
    void setSizeAddedWithBaseSize(Size value){ 
        _size = BASE_SIZE + value;
        assert(_size <= MTU);
    }
    ID getId(){ return _id; }
    void setId(ID id){ _id = id; }
    Type getType(){ return _type; }
    void setType(Type type){ _type = type; }
    RegisterIdValue getRegisterId(){ return _registerId; }
    void setRegisterId(RegisterIdValue registerId){ _registerId = registerId; }
    const Data * getData(){ return _data; }
    void setData(const Data * data, Size length){
        setSizeAddedWithBaseSize(length);
        _data = data;
    }
    Size getDataLength(){ return _size - BASE_SIZE; }
};

class CommandReadRequest : public CommandMessage {
protected:
public:
    CommandReadRequest()
    : CommandMessage(CommandMessage::COMMAND_READ_REQUEST){}
};

class CommandReadResponse : public CommandMessage {
protected:
public:
    CommandReadResponse(RegisterIdValue regId, const Data * data, Size dataLength)
    : CommandMessage(regId, data, dataLength, CommandMessage::COMMAND_READ_RESPONSE){}
};

class CommandWriteRequest : public CommandMessage {
protected:
public:
    CommandWriteRequest(RegisterIdValue regId, const Data * data, Size dataLength)
    : CommandMessage(regId, data, dataLength, CommandMessage::COMMAND_WRITE_REQUEST){}
};

class CommandWriteResponse : public CommandMessage {
protected:
public:
    CommandWriteResponse()
    : CommandMessage(CommandMessage::COMMAND_WRITE_RESPONSE){}
};

};

#endif