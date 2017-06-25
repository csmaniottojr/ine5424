class SmartObject:
    def __init__(self, id, name):
        self._id = id
        self._name = name
        self._services = []

    # Getters
    def get_id(self):
        return self._id
    
    def get_name(self):
        return self._name
    
    def get_services(self):
        return self._services
    
    # Setters
    def set_id(self, id):
        self._id = id
    
    def set_name(self, name):
        self._name = name
    
    # Adders
    def add_service(self, service):
        self._services.append(service)