from ..objects.parameter import Parameter
from enum import Enum

class RegisterMessageType(Enum):
    UNKNOWN_MESSAGE                 = 0
    FIRST_TYPE                      = 1

    REGISTER_REQUEST                = FIRST_TYPE
    REGISTER_RESPONSE               = 2
    REGISTER_OBJECT_REQUEST         = 3
    REGISTER_OBJECT_RESPONSE        = 4
    REGISTER_SERVICE_REQUEST        = 5
    REGISTER_SERVICE_RESPONSE       = 6
    REGISTER_PARAMETER_REQUEST      = 7
    REGISTER_PARAMETER_RESPONSE     = 8
    REGISTER_OPTION_REQUEST         = 9
    REGISTER_OPTION_RESPONSE        = 10
    REGISTER_END_OBJECT_REQUEST     = 11
    REGISTER_END_OBJECT_RESPONSE    = 12

    LAST_TYPE                       = REGISTER_END_OBJECT_RESPONSE

class RegisterMessage(object):
    START_CHAR = ';'
    BASE_SIZE = 7 #START_CHAR[1] + Size[1] + ID[4] + RegisterMessageType[1]
    MTU = 116 - BASE_SIZE #116 vem do IEEE802_15_4::Frame::MTU

    def __init__(self, type=RegisterMessageType.UNKNOWN_MESSAGE):
        self._size = self.__class__.BASE_SIZE
        self._id = -1
        self._type = type
    
    # Getters
    def get_size(self):
        return self._size
    
    def get_id(self):
        return self._id
    
    def get_type(self):
        return self._type
    
    # Setters
    def set_size(self, size):
        self._size = size
        if self._size > self.__class__.MTU:
            raise Exception("RegisterMessage> Tamanho da mensagem passou do limite! %d / %d" % (self._size, self.__class__.MTU))
    
    def set_size_added_with_base_size(self, value):
        self.set_size(self.__class__.BASE_SIZE + value)

    def set_id(self, id):
        self._id = id
    
    def set_type(self, type):
        if not isinstance(type, RegisterMessageType):
            type = RegisterMessageType(type)
        self._type = type

#################################################################
class RegisterRequest(RegisterMessage):
    def __init__(self):
        super(RegisterRequest, self).__init__(RegisterMessageType.REGISTER_REQUEST)

#################################################################
class RegisterResponse(RegisterMessage):
    def __init__(self, isRegistered=False):
        super(RegisterResponse, self).__init__(RegisterMessageType.REGISTER_RESPONSE)
        self._is_registered = isRegistered
        self.set_size_added_with_base_size(1)#bool size
    
    def is_registered(self):
        return self._is_registered
    
    def set_is_registered(self, value):
        self._is_registered = value

#################################################################
class RegisterObjectRequest(RegisterMessage):
    def __init__(self, obj):
        super(RegisterObjectRequest, self).__init__(RegisterMessageType.REGISTER_OBJECT_REQUEST)
        self.set_object(obj)
    
    def get_object_name(self):
        return self._object.get_name()
    
    def get_object(self):
        return self._object
    
    def set_object(self, obj):
        self._object = obj
        self.set_size_added_with_base_size(len(obj.get_name()))

#################################################################
class RegisterObjectResponse(RegisterMessage):
    def __init__(self):
        super(RegisterObjectResponse, self).__init__(RegisterMessageType.REGISTER_OBJECT_RESPONSE)

#################################################################
class RegisterServiceRequest(RegisterMessage):
    def __init__(self, serv):
        super(RegisterServiceRequest, self).__init__(RegisterMessageType.REGISTER_SERVICE_REQUEST)
        self.set_service(serv)
    
    def get_service_name(self):
        return self._service.get_name()
    
    def get_service(self):
        return self._service
    
    def set_service(self, serv):
        self._service = serv
        self.set_size_added_with_base_size(len(serv.get_name()))

#################################################################
class RegisterServiceResponse(RegisterMessage):
    def __init__(self):
        super(RegisterServiceResponse, self).__init__(RegisterMessageType.REGISTER_SERVICE_RESPONSE)

#################################################################
class RegisterParameterRequest(RegisterMessage):
    def __init__(self, param):
        super(RegisterParameterRequest, self).__init__(RegisterMessageType.REGISTER_PARAMETER_REQUEST)
        self._parameter = param
        self._update_size()
    
    def get_parameter(self):
        return self._parameter
    
    def set_parameter(self, param):
        self._parameter = param
        self._update_size()
    
    def get_parameter_name(self):
        return self._parameter.get_name()
    
    def get_register_id(self):
        return self._parameter.get_register_id()
    
    def get_ptype(self):#TODO pensar em um nome melhor!!
        return self._parameter.get_type()
    
    def get_min_value(self):
        return self._parameter.get_min_value()
    
    def get_max_value(self):
        return self._parameter.get_max_value()
    
    def _update_size(self):
        size = Parameter.BASE_SIZE
        size += int(self.get_min_value()[0])
        size += int(self.get_max_value()[0])
        size += len(self.get_parameter_name())
        self.set_size_added_with_base_size(size)

#################################################################
class RegisterParameterResponse(RegisterMessage):
    def __init__(self):
        super(RegisterParameterResponse, self).__init__(RegisterMessageType.REGISTER_PARAMETER_RESPONSE)

#################################################################
class RegisterOptionRequest(RegisterMessage):
    def __init__(self, opt):
        super(RegisterOptionRequest, self).__init__(RegisterMessageType.REGISTER_OPTION_REQUEST)
        self.set_option(opt)
    
    def get_option(self):
        return self._option
    
    def set_option(self, opt):
        self._option = opt
        self.set_size_added_with_base_size(len(opt))
    
#################################################################
class RegisterOptionResponse(RegisterMessage):
    def __init__(self):
        super(RegisterOptionResponse, self).__init__(RegisterMessageType.REGISTER_OPTION_RESPONSE)

#################################################################
class RegisterEndObjectRequest(RegisterMessage):
    def __init__(self):
        super(RegisterEndObjectRequest, self).__init__(RegisterMessageType.REGISTER_END_OBJECT_REQUEST)

#################################################################
class RegisterEndObjectResponse(RegisterMessage):
    def __init__(self):
        super(RegisterEndObjectResponse, self).__init__(RegisterMessageType.REGISTER_END_OBJECT_RESPONSE)
