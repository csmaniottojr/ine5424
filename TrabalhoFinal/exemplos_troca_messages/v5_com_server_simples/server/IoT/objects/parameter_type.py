from enum import Enum

class ParameterTypeType(Enum):
    UNKNOWN             = 0
    INTEGER             = 1
    UNSIGNED_INTEGER    = 2
    SHORT               = 3
    UNSIGNED_SHORT      = 4
    FLOAT               = 5
    UNSIGNED_FLOAT      = 6
    CHAR                = 7
    UNSIGNED_CHAR       = 8
    BOOLEAN             = 9
    COMBO               = 10
    
class ParameterType(object):
    def __init__(self, type=ParameterTypeType.UNKNOWN):
        self._min = bytearray([0x00])
        self._max = bytearray([0x00])
        if not isinstance(type, ParameterTypeType):
            type = ParameterTypeType(type)
        self._type = type
    
    # Getters
    def get_min_value(self):
        return self._min
    
    def get_max_value(self):
        return self._max
    
    def get_type(self):
        return self._type