from enum import Enum

class CommandMessageType(Enum):
    UNKNOWN_MESSAGE                 = 0
    FIRST_TYPE                      = 1

    COMMAND_READ_REQUEST            = FIRST_TYPE
    COMMAND_READ_RESPONSE           = 2
    COMMAND_WRITE_REQUEST           = 3
    COMMAND_WRITE_RESPONSE          = 4

    LAST_TYPE                       = COMMAND_WRITE_RESPONSE

class CommandMessage(object):
    START_CHAR = ':'
    BASE_SIZE = 9 #START_CHAR[1] + Size[1] + ID[4] + CommandMessageType[1] + RegId[2]
    MTU = 116 - BASE_SIZE #116 vem do IEEE802_15_4::Frame::MTU

    def __init__(self, type=CommandMessageType.UNKNOWN_MESSAGE):
        self._size = self.__class__.BASE_SIZE
        self._id = -1
        self.set_type(type)
        self._register_id = -1
        self._data = bytearray()
    
    # Getters
    def get_size(self):
        return self._size
    
    def get_id(self):
        return self._id
    
    def get_type(self):
        return self._type
    
    def get_register_id(self):
        return self._register_id
    
    def get_data(self):
        return self._data
    
    def get_data_length(self):
        return self._size - self.__class__.BASE_SIZE
    
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
        if not isinstance(type, CommandMessageType):
            type = CommandMessageType(type)
        self._type = type

    def set_register_id(self, regId):
        self._register_id = regId;
    
    def set_data(self, data, length):
        self.set_size_added_with_base_size(length)
        self._data = data

#################################################################
class CommandReadRequest(CommandMessage):
    def __init__(self):
        super(CommandReadRequest, self).__init__(CommandMessageType.COMMAND_READ_REQUEST)

#################################################################
class CommandReadResponse(CommandMessage):
    def __init__(self):
        super(CommandReadResponse, self).__init__(CommandMessageType.COMMAND_READ_RESPONSE)

#################################################################
class CommandWriteRequest(CommandMessage):
    def __init__(self):
        super(CommandWriteRequest, self).__init__(CommandMessageType.COMMAND_WRITE_REQUEST)

#################################################################
class CommandWriteResponse(CommandMessage):
    def __init__(self):
        super(CommandWriteResponse, self).__init__(CommandMessageType.COMMAND_WRITE_RESPONSE)
