from IoT.objects.parameter_type import *

class ParameterCombo(ParameterType):
    def __init__(self):
        super(ParameterCombo, self).__init__(ParameterTypeType.COMBO)
        self._options = []
    
    # Getters
    def get_options():
        return self._options

    # Adders
    def add_option(self, option):
        self._options.append(option)