from register.register_message_type import RegisterMessageType
from command.command_serialization import CommandSerialization
from register.register_serialization import RegisterSerialization
from debug.debug_serialization import DebugSerialization 

class MessageValidator(object):

    START_CHARES = RegisterSerialization.START_CHAR + \
        CommandSerialization.START_CHAR + \
        DebugSerialization.START_CHAR

    @staticmethod
    def check_if_message_is_complete(barray):
        length = len(barray)
        if length < 2:
            return False
        
        return RegisterSerialization.is_valid_message(barray, length) or \
            CommandSerialization.is_valid_message(barray, length) or \
            DebugSerialization.is_valid_message(barray, length)

    @staticmethod
    def check_if_is_end_of_register(barray_list):
        last_barray = barray_list[-1]

        msg_type = RegisterSerialization.deserialize_message_type(last_barray)
        return msg_type in RegisterMessageType.end_messages()
    
    @staticmethod
    def check_if_is_valid_start(start):
        return chr(start[0]) in MessageValidator.START_CHARES