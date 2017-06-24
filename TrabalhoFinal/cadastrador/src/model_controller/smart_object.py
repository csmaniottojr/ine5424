from abc import ABCMeta, abstractmethod

class SmartObjectController(metaclass=ABCMeta):


    @abstractmethod
    def save(self, smart_object):
        pass

    @abstractmethod
    def check_if_exists(self, device_id):
        pass
