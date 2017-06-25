from .register_message import *
from ..objects import *
from ..utils.utils import Utils

class RegisterSerialization:

    # Formato das mensagens de Register:
    # Bit 0            1          2       6          7        x   
    #     +------------+----------+-------+----------+--- ~ --+
    #     | START_CHAR | msg size | SO id | msg type |  data  |
    #     +------------+----------+-------+----------+--- ~ --+
    # PS: data se refere aos dados especificos de cada tipo de mensagem

    START_CHAR = RegisterMessage.START_CHAR
    BASE_SIZE = RegisterMessage.BASE_SIZE

    @staticmethod
    def serialize(message):
        result = bytearray()

        result.append(ord(RegisterSerialization.START_CHAR))

        result.append(message.get_size())

        id = Utils.pack_with_byte_order("I", message.get_id())
        result.extend(b for b in id)

        type = message.get_type()
        tmp = Utils.pack_with_byte_order("B", type.value)
        result.extend(tmp)

        if type == RegisterMessageType.REGISTER_RESPONSE:
            result.append(message.is_registered())
        elif type == RegisterMessageType.REGISTER_OBJECT_REQUEST:
            name = str.encode(message.get_object_name())
            result.extend(b for b in name)
        elif type == RegisterMessageType.REGISTER_SERVICE_REQUEST:
            name = str.encode(message.get_service_name())
            result.extend(b for b in name)
        elif type == RegisterMessageType.REGISTER_PARAMETER_REQUEST:
            tmp = message.get_ptype().value
            ptype = Utils.pack_with_byte_order("B", tmp)
            result.extend(ptype)
            
            regId = Utils.pack_with_byte_order("H", message.get_register_id())
            result.extend(b for b in regId)
            
            tmp = message.get_min_value()
            if int(tmp[0]) > 0:
                result.extend(tmp[1:])
            tmp = message.get_max_value()
            if int(tmp[0]) > 0:
                result.extend(tmp[1:])

            name = str.encode(message.get_parameter_name())
            result.extend(b for b in name)
        elif type == RegisterMessageType.REGISTER_OPTION_REQUEST:
            opt = str.encode(message.get_option())
            result.extend(b for b in opt)

        return result

    @staticmethod
    def deserialize(message):
        result = None

        if chr(message[0]) != RegisterSerialization.START_CHAR:
            return result
        
        message = message[1:]#ignora o START_CHAR
        i = 0

        size = int(message[i])
        i += 1

        id = Utils.unpack_with_byte_order("I", message[i:(i+4)])
        i += 4

        type = int(message[i])
        i += 1

        if type == RegisterMessageType.REGISTER_REQUEST.value:
            result = RegisterRequest()
        elif type == RegisterMessageType.REGISTER_RESPONSE.value:
            result = RegisterResponse() 
            tmp = Utils.unpack_with_byte_order("?", message[i:(i+1)])
            result.set_is_registered(tmp)
            i += 1
        elif type == RegisterMessageType.REGISTER_OBJECT_REQUEST.value:
            length = (size - RegisterSerialization.BASE_SIZE)
            name = message[i:(i+length)].decode("utf-8")
            i += length

            so = smart_object.SmartObject(id, name)
            result = RegisterObjectRequest(so)
        elif type == RegisterMessageType.REGISTER_OBJECT_RESPONSE.value:
            result = RegisterObjectResponse()
        elif type == RegisterMessageType.REGISTER_SERVICE_REQUEST.value:
            length = (size - RegisterSerialization.BASE_SIZE)
            name = message[i:(i+length)].decode("utf-8")
            i += length

            serv = service.Service(name)
            result = RegisterServiceRequest(serv)
        elif type == RegisterMessageType.REGISTER_SERVICE_RESPONSE.value:
            result = RegisterServiceResponse()
        elif type == RegisterMessageType.REGISTER_PARAMETER_REQUEST.value:
            ptype = int(message[i])
            i += 1

            regId = Utils.unpack_with_byte_order("H", message[i:(i+2)])
            i += 2

            paramType = None
            if ptype == parameter_type.ParameterTypeType.BOOLEAN.value:
                paramType = parameter_boolean.ParameterBoolean()
                i += 2
            elif ptype == parameter_type.ParameterTypeType.FLOAT.value:
                min = Utils.unpack_with_byte_order("f", message[i:(i+4)])
                i += 4

                max = Utils.unpack_with_byte_order("f", message[i:(i+4)])
                i += 4

                paramType = parameter_float.ParameterFloat(min, max)
            elif ptype == parameter_type.ParameterTypeType.COMBO.value:
                paramType = parameter_combo.ParameterCombo()

            length = (size - i - 1)
            name = message[i:(i+length)].decode("utf-8")
            i += length

            param = parameter.Parameter(name, regId, paramType)
            result = RegisterParameterRequest(param)
        elif type == RegisterMessageType.REGISTER_PARAMETER_RESPONSE.value:
            result = RegisterParameterResponse()
        elif type == RegisterMessageType.REGISTER_OPTION_REQUEST.value:
            length = (size - RegisterSerialization.BASE_SIZE)
            opt = message[i:(i+length)].decode("utf-8")
            i += length

            result = RegisterOptionRequest(opt)
        elif type == RegisterMessageType.REGISTER_OPTION_RESPONSE.value:
            result = RegisterOptionResponse()
        elif type == RegisterMessageType.REGISTER_END_OBJECT_REQUEST.value:
            result = RegisterEndObjectRequest()
        elif type == RegisterMessageType.REGISTER_END_OBJECT_RESPONSE.value:
            result = RegisterEndObjectResponse()
        
        if result != None:
            result.set_size(size)
            result.set_id(id)
            result.set_type(type)

        return result