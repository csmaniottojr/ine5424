from collections import defaultdict
from threading import Lock

from utils.observer import Observer
from utils.utils import Utils
from command.command_serialization import CommandSerialization
from command.command_message_type import CommandMessageType

class CommandManager(Observer):
    def __init__(self):
        Observer.__init__(self)
        self.commands = defaultdict(lambda: [])
        self.serialization = CommandSerialization()
        self.mutex = Lock()
    
    def update(self, observable, data):
        if chr(data[0]) == CommandSerialization.START_CHAR:
            emote_id = CommandSerialization.deserialize_emote_id(data)
            type = CommandSerialization.deserialize_message_type(data)

            Utils.debug("Mensagem de comando recebida: {id: %d, type: %s}" % \
                (emote_id, type))
                
            with self.mutex:
                self.commands[emote_id].append(data)
    
    # msg_type que esta esperando, COMMAND_READ_RESPONSE por exemplo...
    def get_response(self, emote_id, register_id, msg_type):
        index = -1
        if not isinstance(msg_type, CommandMessageType):
            msg_type = CommandMessageType( int(msg_type) )
        with self.mutex:
            for idx, command in enumerate(self.commands[emote_id]):
                _reg_id = CommandSerialization.deserialize_register_id(command)
                _msg_type = CommandSerialization.deserialize_message_type(command)
                if _reg_id == register_id and _msg_type == msg_type:
                    index = idx
                    break
        if index != -1:
            return self.commands[emote_id].pop(index)
        else:
            return bytearray()