from utils.utils import Utils
from .command_message_type import CommandMessageType

class CommandSerialization(object):
    
    # Formato das mensagens de Register:
    # Bit 0            1          2       6          7        x   
    #     +------------+----------+-------+----------+--- ~ --+
    #     | START_CHAR | msg size | SO id | msg type |  data  |
    #     +------------+----------+-------+----------+--- ~ --+
    # PS: data se refere aos dados especificos de cada tipo de mensagem

    START_CHAR = ':'

    @staticmethod
    def is_valid_message(message, length):
        return chr(message[0]) == CommandSerialization.START_CHAR and \
            CommandSerialization.deserialize_size(message) == length

    @staticmethod
    def deserialize_size(barray):
        return int(barray[1])
    
    @staticmethod
    def deserialize_emote_id(barray):
        return Utils.unpack_with_byte_order("I", barray[2:6])
    
    @staticmethod
    def deserialize_message_type(barray):
        return CommandMessageType(int(barray[6]))
    
    @staticmethod
    def serialize(params):
        pass

    @staticmethod
    def deserialize(barray):
        pass