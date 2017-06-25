from utils.observer import Observer
from command.command_serialization import CommandSerialization

class CommandManager(Observer):
    def __init__(self):
        Observer.__init__(self)
    
    def update(self, observable, data):
        if chr(data[0]) == CommandSerialization.START_CHAR:
            print("Mensagem de comando recebida: %s" % \
                ([ "0x%02x" % b for b in data ]))