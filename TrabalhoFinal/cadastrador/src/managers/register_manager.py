# encoding: utf-8
from collections import defaultdict

from utils.observer import Observer
from utils.utils import Utils

from register.register_serialization import RegisterSerialization
from utils.message_validator import MessageValidator
from register.register_message_responser import RegisterMessageResponser

"""
    Classe responsável por gerenciar as mensagens de registro
    vindas dos objetos inteligentes.
"""
class RegisterManager(Observer):
    def __init__(self, model_controller):
        Observer.__init__(self)
        self.registers = defaultdict(lambda: [])
        self.responder = RegisterMessageResponser(model_controller)
        self.serialization = RegisterSerialization(model_controller)
    
    def update(self, observable, data):
        if chr(data[0]) == RegisterSerialization.START_CHAR:
            emote_id = RegisterSerialization.deserialize_emote_id(data)
            type = RegisterSerialization.deserialize_message_type(data);

            Utils.debug("Mensagem de registro recebida: {id: %d, type: %s}" % \
                (emote_id, type))
            
            self.registers[emote_id].append(data)

            #TODO melhorar isso...
            observable.write(self.responder.response(data))

            if MessageValidator.check_if_is_end_of_register(self.registers[emote_id]):
                self.serialization.deserialize(self.registers[emote_id])
                del self.registers[emote_id]