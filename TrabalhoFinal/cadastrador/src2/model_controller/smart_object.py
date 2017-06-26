from abc import ABCMeta, abstractmethod

class SmartObjectController(metaclass=ABCMeta):


    @abstractmethod
    def save(self, smart_object):
        pass

    @abstractmethod
    def check_if_exists(self, device_id):
        pass

    @abstractmethod
    def list(self):
        pass

    @abstractmethod
    def get_by_id(self, _id):
        pass
