from utils.observer import Observer
from register.register_serialization import RegisterSerialization
from collections import defaultdict
from utils.message_validator import MessageValidator
from register.register_message_responser import RegisterMessageResponser

class RegisterManager(Observer):
    def __init__(self, model_controller):
        Observer.__init__(self)
        self.registers = defaultdict(lambda: [])
        self.responder = RegisterMessageResponser(model_controller)
        self.serialization = RegisterSerialization(model_controller)
    
    def update(self, observable, data):
        if chr(data[0]) == RegisterSerialization.START_CHAR:
            print("Mensagem de registro recebida: %s" % \
                ([ "0x%02x" % b for b in data ]))
            
            emote_id = RegisterSerialization.deserialize_emote_id(data)
            self.registers[emote_id].append(data)

            #TODO melhorar isso...
            observable.write(self.responder.response(data))

            if MessageValidator.check_if_is_end_of_register(self.registers[emote_id]):
                self.serialization.deserialize(self.registers[emote_id])
                del self.registers[emote_id]