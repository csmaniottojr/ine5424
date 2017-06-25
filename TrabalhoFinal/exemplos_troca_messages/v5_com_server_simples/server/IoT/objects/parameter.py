class Parameter:
    BASE_SIZE = 3 #Type[1] + RegId[2]

    def __init__(self, name, register_id, parameter_type):
        self._name = name
        self._register_id = register_id
        self._parameter_type = parameter_type
    
    # Getters
    def get_name(self):
        return self._name
    
    def get_register_id(self):
        return self._register_id
    
    def get_type(self):
        return self._parameter_type.get_type()
    
    def get_min_value(self):
        return self._parameter_type.get_min_value()
    
    def get_max_value(self):
        return self._parameter_type.get_max_value()
    
    def get_parameter_type(self):
        return self._parameter_type