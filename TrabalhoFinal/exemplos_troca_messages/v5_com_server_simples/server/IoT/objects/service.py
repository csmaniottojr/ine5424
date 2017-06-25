class Service:
    def __init__(self, name):
        self._name = name
        self._parameters = []
    
    # Getters
    def get_name(self):
        return self._name
    
    def get_parameters(self):
        return self._parameters
    
    # Setters
    def set_name(self, name):
        self._name = name
    
    # Adders
    def add_parameter(self, parameter):
        self._parameters.append(parameter)