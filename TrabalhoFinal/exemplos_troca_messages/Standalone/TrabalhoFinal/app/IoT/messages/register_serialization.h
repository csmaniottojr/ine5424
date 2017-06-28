#ifndef register_serialization_h__
#define register_serialization_h__

#include "../objects/smartobject.h"
#include "../objects/parametertype.h"
#include "../objects/parameter_boolean.h"
#include "../objects/parameter_combo.h"
#include "../objects/parameter_float.h"
#include "../objects/parameter_integer.h"
#include "register_message.h"

using namespace EPOS;

namespace IoT {

class RegisterSerialization {
public:
    typedef RegisterMessage::Size Size;
    typedef RegisterMessage::ID ID;
    typedef RegisterMessage::Type Type;
    typedef RegisterParameterRequest::RegisterIdValue RegisterIdValue;

    static const unsigned char BASE_SIZE = RegisterMessage::BASE_SIZE;
public:

    // Formato das mensagens de Register:
    // Byte 0            1          2       6          7        x   
    //      +------------+----------+-------+----------+--- ~ --+
    //      | START_CHAR | msg size | SO id | msg type |  data  |
    //      +------------+----------+-------+----------+--- ~ --+
    // PS: data se refere aos dados especificos de cada tipo de mensagem

    static const char * serialize(RegisterMessage * message){
        unsigned int index = 0;
        Size length = message->getSize();
        char * result = new char[length];
        memset(result, '\0', length);

        result[index++] = RegisterMessage::START_CHAR;
        result[index++] = message->getSize();
        ((ID*) &result[index])[0] = message->getId();
        index += sizeof(ID);
        result[index++] = message->getType();

        switch(message->getType()){
            case RegisterMessage::REGISTER_REQUEST:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_RESPONSE:{
                // Formato do data:
                // Byte 0              1
                //      +------ ~ -----+
                //      | isRegistered |
                //      +------ ~ -----+
                RegisterResponse * res = reinterpret_cast<RegisterResponse*>(message);
                result[index++] = res->isRegistered();
                break;
            }case RegisterMessage::REGISTER_OBJECT_REQUEST:{
                // Formato do data:
                // Byte 0          x
                //      +---- ~ ---+
                //      | obj name |
                //      +---- ~ ---+
                // PS: x depende do tamanho do nome [strlen]
                RegisterObjectRequest * req = reinterpret_cast<RegisterObjectRequest*>(message);
                auto objectName = req->getObjectName();
                strncpy(&result[index], objectName, strlen(objectName));
                index += strlen(objectName);
                break;
            }case RegisterMessage::REGISTER_OBJECT_RESPONSE:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_SERVICE_REQUEST:{
                // Formato do data:
                // Byte 0              x
                //      +------ ~ -----+
                //      | service name |
                //      +------ ~ -----+
                // PS: x depende do tamanho do nome [strlen]
                RegisterServiceRequest * req = reinterpret_cast<RegisterServiceRequest*>(message);
                auto serviceName = req->getServiceName();
                strncpy(&result[index], serviceName, strlen(serviceName));
                index += strlen(serviceName);
                break;
            }case RegisterMessage::REGISTER_SERVICE_RESPONSE:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_PARAMETER_REQUEST:{
                // Formato do data:
                // Byte 0            1        3           4           x           y            z
                //      +------------+--------+-----------+---- ~ ----+---- ~ ----+----- ~ ----+
                //      | param type | reg id | read_only | min value | max value | param name |
                //      +------------+--------+-----------+---- ~ ----+---- ~ ----+----- ~ ----+
                // PS: x e y dependem do tipo do parametro [float=integer=4, bool=1]
                //     z depende do tamanho do nome [strlen]
                RegisterParameterRequest * req = 
                    reinterpret_cast<RegisterParameterRequest*>(message);

                result[index++] = req->getType();
                ((RegisterIdValue*) &result[index])[0] = req->getRegisterId();
                index += sizeof(RegisterIdValue);
                ((bool*) &result[index++])[0] = req->isReadOnly();

                const char * tmp = req->getMinValue();
                if(tmp != 0){
                    memcpy(&result[index], &tmp[1], (unsigned char) tmp[0]);
                    index += (unsigned char) tmp[0];
                }
                tmp = req->getMaxValue();
                if(tmp != 0){
                    memcpy(&result[index], &tmp[1], (unsigned char) tmp[0]);
                    index += (unsigned char) tmp[0];
                }

                auto parameterName = req->getParameterName();
                strncpy(&result[index], parameterName, strlen(parameterName));
                index += strlen(parameterName);
                break;
            }case RegisterMessage::REGISTER_PARAMETER_RESPONSE:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_OPTION_REQUEST:{
                // Formato do data:
                // Byte 0        x
                //      +--- ~ --+
                //      | option |
                //      +--- ~ --+
                // PS: x depende do tamanho da option [strlen]
                RegisterOptionRequest * req = reinterpret_cast<RegisterOptionRequest*>(message);
                auto option = req->getOption();
                strncpy(&result[index], option, strlen(option));
                index += strlen(option);
                break;
            }case RegisterMessage::REGISTER_OPTION_RESPONSE:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_END_OBJECT_REQUEST:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_END_OBJECT_RESPONSE:{
                // Não possui data
                break;
            }default: break;
        }
        return result;
    }

    static RegisterMessage* deserialize(const char * msg){
        RegisterMessage *result = 0;

        if(msg[0] != RegisterMessage::START_CHAR){
            return result;
        }

        msg = &msg[1];//ignora o START_CHAR
        unsigned int index = 0;

        Size size = (Size) msg[index++];
        ID id = *((ID*) &msg[index]);
        index += sizeof(ID);
        Type type = (Type) msg[index++];

        switch(type){
            case RegisterMessage::REGISTER_REQUEST:{
                RegisterRequest *req = new RegisterRequest();
                result = req;
                break;
            }case RegisterMessage::REGISTER_RESPONSE:{
                RegisterResponse *res = new RegisterResponse();

                res->setIsRegistered((bool) msg[index++]);
                result = res;
                break;
            }case RegisterMessage::REGISTER_OBJECT_REQUEST:{
                unsigned char length = (size - BASE_SIZE);

                char * name = new char[length+1];
                memset(name, '\0', length+1);
                memcpy(name, &msg[index], length);
                index += length;

                SmartObject * object = new SmartObject(name);
                object->setId(id);
                RegisterObjectRequest *req = new RegisterObjectRequest(object);
                result = req;
                break;
            }case RegisterMessage::REGISTER_OBJECT_RESPONSE:{
                RegisterObjectResponse * res = new RegisterObjectResponse();
                result = res;
                break;
            }case RegisterMessage::REGISTER_SERVICE_REQUEST:{
                unsigned char length = (size - BASE_SIZE);

                char * name = new char[length+1];
                memset(name, '\0', length+1);
                memcpy(name, &msg[index], length);
                index += length;

                Service * service = new Service(name);
                RegisterServiceRequest *req = new RegisterServiceRequest(service);
                result = req;
                break;
            }case RegisterMessage::REGISTER_SERVICE_RESPONSE:{
                RegisterServiceResponse * res = new RegisterServiceResponse();
                result = res;
                break;
            }case RegisterMessage::REGISTER_PARAMETER_REQUEST:{
                ParameterType::Type pType = (ParameterType::Type) msg[index++];
                RegisterIdValue registerId = *((RegisterIdValue*) &msg[index]);
                index += sizeof(RegisterIdValue);
                bool read_only = *((bool*) &msg[index++]);

                ParameterType * parameterType = 0;
                switch(pType){
                    case ParameterType::BOOLEAN:{
                        ParameterBoolean * pBool = new ParameterBoolean();
                        index += 2;//pode simplesmente ignorar o min e max (0 e 1)
                        parameterType = pBool;
                        break;
                    }case ParameterType::FLOAT:{
                        ParameterFloat * pFloat = new ParameterFloat();

                        pFloat->setMinValue( *((float*) &msg[index]) );
                        index += ParameterFloat::VALUE_SIZE;
                        pFloat->setMaxValue( *((float*) &msg[index]) );
                        index += ParameterFloat::VALUE_SIZE;

                        parameterType = pFloat;
                        break;
                    }case ParameterType::INTEGER:{
                        ParameterInteger * pInteger = new ParameterInteger();

                        pInteger->setMinValue( *((int*) &msg[index]) );
                        index += ParameterInteger::VALUE_SIZE;
                        pInteger->setMaxValue( *((int*) &msg[index]) );
                        index += ParameterInteger::VALUE_SIZE;

                        parameterType = pInteger;
                        break;
                    }case ParameterType::COMBO:{
                        ParameterCombo * pCombo = new ParameterCombo();
                        parameterType = pCombo;
                        break;
                    }default: break;
                };
                unsigned char length = (size - index - 1);

                char * name = new char[length+1];
                memset(name, '\0', length+1);
                memcpy(name, &msg[index], length);
                index += length;

                Parameter * parameter = new Parameter(name, registerId, read_only, parameterType);
                RegisterParameterRequest * req = new RegisterParameterRequest(parameter);
                result = req;
                break;
            }case RegisterMessage::REGISTER_PARAMETER_RESPONSE:{
                RegisterParameterResponse * res = new RegisterParameterResponse();
                result = res;
                break;
            }case RegisterMessage::REGISTER_OPTION_REQUEST:{
                unsigned char length = (size - BASE_SIZE);

                char * option = new char[length+1];
                memset(option, '\0', length+1);
                memcpy(option, &msg[index], length);
                index += length;

                RegisterOptionRequest *req = new RegisterOptionRequest(option);
                result = req;
                break;
            }case RegisterMessage::REGISTER_OPTION_RESPONSE:{
                RegisterOptionResponse * res = new RegisterOptionResponse();
                result = res;
                break;
            }case RegisterMessage::REGISTER_END_OBJECT_REQUEST:{
                RegisterEndObjectRequest * req = new RegisterEndObjectRequest();
                result = req;
                break;
            }case RegisterMessage::REGISTER_END_OBJECT_RESPONSE:{
                RegisterEndObjectResponse * res = new RegisterEndObjectResponse();
                result = res;
                break;
            }default: break;
        };
        if(result != 0){
            result->setSize(size);
            result->setId(id);
            result->setType(type);
        }
        return result;
    }

    /**
     * Deserializa apenas as informações basicas de uma mensagem
     * (size, id e type).
     */
    static RegisterMessage* simpleDeserialize(const char * msg){
        if(msg[0] != RegisterMessage::START_CHAR){
            return 0;
        }

        msg = &msg[1];//ignora o START_CHAR
        unsigned int index = 0;

        Size size = (Size) msg[index++];
        ID id = *((ID*) &msg[index]);
        index += sizeof(ID);
        Type type = (Type) msg[index++];

        RegisterMessage *result = new RegisterMessage();
        result->setSize(size);
        result->setId(id);
        result->setType(type);
        return result;
    }
};

};
#endif