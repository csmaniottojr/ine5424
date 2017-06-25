from IoT.objects.parameter_type import *

class ParameterBoolean(ParameterType):
    def __init__(self):
        super(ParameterBoolean, self).__init__(ParameterTypeType.BOOLEAN)
        self.set_min_value(False)
        self.set_max_value(True)
    
    # Setters
    def set_min_value(self, min):
        min = bool(min)
        self._min = bytearray([0x01, min])
    
    def set_max_value(self, max):
        max = bool(max)
        self._max = bytearray([0x01, max])