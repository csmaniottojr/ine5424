from utils.observer import Observer
from debug.debug_serialization import DebugSerialization

class DebugManager(Observer):
    def __init__(self):
        Observer.__init__(self)
    
    def update(self, observable, data):
        if chr(data[0]) == DebugSerialization.START_CHAR:
            print("Mensagem de comentario recebida: %s" % data.decode("utf-8"))