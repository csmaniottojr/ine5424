#ifndef command_serialization_h__
#define command_serialization_h__

#include "../objects/smartobject.h"
#include "../objects/parametertype.h"
#include "../objects/parameter_boolean.h"
#include "../objects/parameter_combo.h"
#include "../objects/parameter_float.h"
#include "command_message.h"

using namespace EPOS;

namespace IoT {

//TODO testar!
class CommandSerialization {
public:
    typedef CommandMessage::Size Size;
    typedef CommandMessage::ID ID;
    typedef CommandMessage::Type Type;
    typedef CommandMessage::RegisterIdValue RegisterIdValue;

    static const unsigned char BASE_SIZE = CommandMessage::BASE_SIZE;
public:
    static const char * serialize(CommandMessage * message){
        Size index = 0;
        Size length = message->getSize();
        char * result = new char[length];
        memset(result, '\0', length);

        // Formato das mensagens de Command:
        // Bit 0            1          2       6          7        x   
        //     +------------+----------+-------+----------+--- ~ --+
        //     | START_CHAR | msg size | SO id | msg type |  data  |
        //     +------------+----------+-------+----------+--- ~ --+

        result[index++] = CommandMessage::START_CHAR;

        result[index++] = message->getSize();

        ((ID*) &result[index])[0] = message->getId();
        index += sizeof(ID);

        Type type = message->getType();
        result[index++] = type;

        ((RegisterIdValue*) &result[index])[0] = message->getRegisterId();
        index += sizeof(RegisterIdValue);
        
        length = message->getDataLength();
        if(length > 0 && (type == CommandMessage::COMMAND_WRITE_REQUEST ||
            type == CommandMessage::COMMAND_READ_RESPONSE)){
            memcpy(&result[index], message->getData(), length);
            index += length;
        }
        return result;
    }

    static CommandMessage* deserialize(const char * msg){
        CommandMessage *result = 0; 

        if(msg[0] != CommandMessage::START_CHAR){
            return result;
        }

        result = new CommandMessage();
        msg = &msg[1];//ignora o START_CHAR
        Size index = 0;

        Size size = (Size) msg[index++];
        result->setSize(size);

        ID id = *((ID*) &msg[index]);
        result->setId(id);
        index += sizeof(ID);

        Type type = (Type) msg[index++];
        result->setType(type);

        RegisterIdValue registerId = *((RegisterIdValue*) &msg[index]);
        result->setRegisterId(registerId);
        index += sizeof(RegisterIdValue);

        Size length = result->getDataLength();
        if(length > 0 && (type == CommandMessage::COMMAND_WRITE_REQUEST ||
            type == CommandMessage::COMMAND_READ_RESPONSE)){
            auto data = ((CommandMessage::Data*) &msg[index]);
            result->setData(data, length);
        }
        return result;
    }

    /**
     * Deserializa apenas as informações basicas de uma mensagem
     * (size, id e type).
     */
    static CommandMessage* simpleDeserialize(const char * msg){
        CommandMessage *result = 0; 

        if(msg[0] != CommandMessage::START_CHAR){
            return result;
        }

        result = new CommandMessage();
        msg = &msg[1];//ignora o START_CHAR
        Size index = 0;

        Size size = (Size) msg[index++];
        result->setSize(size);

        ID id = *((ID*) &msg[index]);
        result->setId(id);
        index += sizeof(ID);

        Type type = (Type) msg[index++];
        result->setType(type);

        return result;
    }
};

};

#endif