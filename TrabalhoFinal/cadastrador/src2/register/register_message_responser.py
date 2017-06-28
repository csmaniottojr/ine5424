# encoding: utf-8
import sys
from .register_serialization import RegisterSerialization
from .register_message_type import RegisterMessageType

"""
    Classe responsável por responder as mensagens
    de registro vindas de um objeto inteligente.
"""
class RegisterMessageResponser(object):

    def __init__(self, model_controller):
        self.model_controller = model_controller

    def response(self, msg):
        barray = bytearray()

        barray.append(int(msg[0]))
        barray.append(RegisterSerialization.BASE_SIZE)
        emote_id = msg[2:6]
        barray.extend(emote_id)

        msg_type = RegisterSerialization.deserialize_message_type(msg)

        if msg_type == RegisterMessageType.REGISTER_REQUEST:
            id = int.from_bytes(emote_id, byteorder=sys.byteorder)
            barray.append(RegisterMessageType.REGISTER_RESPONSE.value)
            
            barray.append(int(self.model_controller.check_if_exists(id)))
            
            barray[1] = int(barray[1]) + 1#atualiza o size

        elif msg_type == RegisterMessageType.REGISTER_OBJECT_REQUEST:
            barray.append(RegisterMessageType.REGISTER_OBJECT_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_SERVICE_REQUEST:
            barray.append(RegisterMessageType.REGISTER_SERVICE_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_PARAMETER_REQUEST:
            barray.append(RegisterMessageType.REGISTER_PARAMETER_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_OPTION_REQUEST:
            barray.append(RegisterMessageType.REGISTER_OPTION_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_END_OBJECT_REQUEST:
            barray.append(RegisterMessageType.REGISTER_END_OBJECT_RESPONSE.value)
        
        return barray
