from IoT.objects.parameter_type import *
from ..utils.utils import Utils
import struct

class ParameterFloat(ParameterType):
    def __init__(self, min, max):
        super(ParameterFloat, self).__init__(ParameterTypeType.FLOAT)
        self.set_min_value(min)
        self.set_max_value(max)
    
    # Setters
    def set_min_value(self, min):
        min = float(min)
        self._min = bytearray(Utils.pack_with_byte_order("f", min))
        self._min.insert(0, 0x04)#deve comecar com o size de float
    
    def set_max_value(self, max):
        max = float(max)
        self._max = bytearray(Utils.pack_with_byte_order("f", max))
        self._max.insert(0, 0x04)#deve comecar com o size de float