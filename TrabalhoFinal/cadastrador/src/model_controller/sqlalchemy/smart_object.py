from model_controller.smart_object import SmartObjectController
from models import SmartObject

class SmartObjectSQLAController(SmartObjectController):


    def __init__(self, session):
        self.session = session

    def save(self, smart_object):
        self.session.add(smart_object)
        self.session.commit()


    def check_if_exists(self, device_id):
        smart_obj = self.session.query(SmartObject).filter(
            SmartObject.device_id == device_id).first()

        return smart_obj is not None
