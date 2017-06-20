#ifndef serialization_register_h__
#define serialization_register_h__

#include "../objects/smartobject.h"
#include "../objects/parametertype.h"
#include "../objects/parameter_boolean.h"
#include "../objects/parameter_combo.h"
#include "../objects/parameter_float.h"
#include "register_message.h"

using namespace EPOS;

namespace IoT {

class SerializationRegister {
public:
    static const unsigned char BASE_SIZE = RegisterMessage::BASE_SIZE;
    static const unsigned char ID_SIZE = RegisterMessage::ID_SIZE;

    typedef RegisterMessage::Size Size;
    typedef RegisterMessage::ID ID;
    typedef RegisterMessage::Type Type;
public:
    static const char * serialize(RegisterMessage * message){
        int length = message->getSize();
        char * result = new char[length+1];
        memset(result, '\0', length+1);

        // Formato das mensagens de Register:
        // Bit 0           1        5          6        x   
        //     +-----------+--------+----------+--- ~ --+
        //     | msg size  | msg id | msg type |  data  |
        //     +-----------+--------+----------+--- ~ --+
        // PS: data se refere aos dados especificos de cada tipo de mensagem

        result[0] = message->getSize();
        memcpy(&result[1], message->getId(), ID_SIZE);
        result[ID_SIZE+1] = message->getType();

        switch(message->getType()){
            case RegisterMessage::REGISTER_REQUEST:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_RESPONSE:{
                // Formato do data:
                // Bit 0              1
                //     +------ ~ -----+
                //     | isRegistered |
                //     +------ ~ -----+
                RegisterResponse * res = reinterpret_cast<RegisterResponse*>(message);
                result[BASE_SIZE] = res->isRegistered();
                break;
            }case RegisterMessage::REGISTER_OBJECT_REQUEST:{
                // Formato do data:
                // Bit 0          x
                //     +---- ~ ---+
                //     | obj name |
                //     +---- ~ ---+
                // PS: x depende do tamanho do nome [strlen]
                RegisterObjectRequest * req = reinterpret_cast<RegisterObjectRequest*>(message);
                auto objectName = req->getObjectName();
                strncpy(&result[BASE_SIZE], objectName, strlen(objectName));
                break;
            }case RegisterMessage::REGISTER_OBJECT_RESPONSE:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_SERVICE_REQUEST:{
                // Formato do data:
                // Bit 0              x
                //     +------ ~ -----+
                //     | service name |
                //     +------ ~ -----+
                // PS: x depende do tamanho do nome [strlen]
                RegisterServiceRequest * req = reinterpret_cast<RegisterServiceRequest*>(message);
                auto serviceName = req->getServiceName();
                strncpy(&result[BASE_SIZE], serviceName, strlen(serviceName));
                break;
            }case RegisterMessage::REGISTER_SERVICE_RESPONSE:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_PARAMETER_REQUEST:{
                // Formato do data:
                // Bit 0            1        3           x           y          z
                //     +------------+--------+---- ~ ----+---- ~ ----+----- ~ ----+
                //     | param type | reg id | min value | max value | param name |
                //     +------------+--------+---- ~ ----+---- ~ ----+----- ~ ----+
                // PS: x e y dependem do tipo do parametro [float=4, bool=1]
                //     z depende do tamanho do nome [strlen]
                RegisterParameterRequest * req = 
                    reinterpret_cast<RegisterParameterRequest*>(message);

                int i = BASE_SIZE;
                result[i] = req->getType();
                i += 1;
                ((unsigned short*) &result[i])[0] = req->getRegisterId();
                i += 2;

                const char * tmp = req->getMinValue();
                if(tmp != 0){
                    memcpy(&result[i], &tmp[1], (unsigned char) tmp[0]);
                    i += (unsigned char) tmp[0];
                }
                tmp = req->getMaxValue();
                if(tmp != 0){
                    memcpy(&result[i], &tmp[1], (unsigned char) tmp[0]);
                    i += (unsigned char) tmp[0];
                }

                auto parameterName = req->getParameterName();
                strncpy(&result[i], parameterName, strlen(parameterName));
                break;
            }case RegisterMessage::REGISTER_PARAMETER_RESPONSE:{
                // Não possui data
                break;
            }case RegisterMessage::REGISTER_OPTION_REQUEST:{
                // Formato do data:
                // Bit 0        x
                //     +--- ~ --+
                //     | option |
                //     +--- ~ --+
                // PS: x depende do tamanho da option [strlen]
                RegisterOptionRequest * req = reinterpret_cast<RegisterOptionRequest*>(message);
                auto option = req->getOption();
                strncpy(&result[BASE_SIZE], option, strlen(option));
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

        Size size = (Size) msg[0];
        ID id;
        memcpy(id, &msg[1], ID_SIZE);
        Type type = (Type) msg[ID_SIZE+1];

        switch(type){
            case RegisterMessage::REGISTER_REQUEST:{
                RegisterRequest *req = new RegisterRequest();
                result = req;
                break;
            }case RegisterMessage::REGISTER_RESPONSE:{
                RegisterResponse *res = new RegisterResponse();

                res->setIsRegistered((bool) msg[BASE_SIZE]);
                result = res;
                break;
            }case RegisterMessage::REGISTER_OBJECT_REQUEST:{
                unsigned char length = (size - BASE_SIZE);

                char * name = new char[length+1];
                memset(name, '\0', length+1);
                memcpy(name, &msg[BASE_SIZE], length);

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
                memcpy(name, &msg[BASE_SIZE], length);

                Service * service = new Service(name);
                RegisterServiceRequest *req = new RegisterServiceRequest(service);
                result = req;
                break;
            }case RegisterMessage::REGISTER_SERVICE_RESPONSE:{
                RegisterServiceResponse * res = new RegisterServiceResponse();
                result = res;
                break;
            }case RegisterMessage::REGISTER_PARAMETER_REQUEST:{
                int i = BASE_SIZE;
                ParameterType::Type pType = (ParameterType::Type) msg[i];
                i += 1;
                unsigned short registerId = *((unsigned short*) &msg[i]);
                i += 2;

                ParameterType * parameterType = 0;
                switch(pType){
                    case ParameterType::BOOLEAN:{
                        ParameterBoolean * pBool = new ParameterBoolean();
                        i += 2;
                        parameterType = pBool;
                        break;
                    }case ParameterType::FLOAT:{
                        ParameterFloat * pFloat = new ParameterFloat();

                        pFloat->setMinValue( *((float*) &msg[i]) );
                        i += ParameterFloat::VALUE_SIZE;
                        pFloat->setMaxValue( *((float*) &msg[i]) );
                        i += ParameterFloat::VALUE_SIZE;

                        parameterType = pFloat;
                        break;
                    }case ParameterType::COMBO:{
                        ParameterCombo * pCombo = new ParameterCombo();
                        parameterType = pCombo;
                        break;
                    }default: break;
                };
                unsigned char length = (size - i);

                char * name = new char[length+1];
                memset(name, '\0', length+1);
                memcpy(name, &msg[i], length);

                Parameter * parameter = new Parameter(name, registerId, parameterType);
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
                memcpy(option, &msg[BASE_SIZE], length);

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
};

};
#endif