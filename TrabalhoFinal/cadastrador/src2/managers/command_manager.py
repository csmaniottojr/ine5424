from collections import defaultdict
from threading import Lock

from utils.observer import Observer
from command.command_serialization import CommandSerialization

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

            print("Mensagem de comando recebida: {id: %d, type: %s}" % \
                (emote_id, type))
                
            with self.mutex:
                self.commands[emote_id].append(data)
    
    # msg_type que esta esperando, COMMAND_READ_RESPONSE por exemplo...
    def get_response(self, emote_id, register_id, msg_type):
        index = -1
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