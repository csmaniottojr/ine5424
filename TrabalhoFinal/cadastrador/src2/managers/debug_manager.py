# encoding: utf-8
from utils.observer import Observer
from utils.utils import Utils
from debug.debug_serialization import DebugSerialization

""" 
    Classe responsável por gerenciar as mensagens de
    Debug vindas dos objetos inteligentes.
"""
class DebugManager(Observer):
    def __init__(self):
        Observer.__init__(self)
    
    def update(self, observable, data):
        if chr(data[0]) == DebugSerialization.START_CHAR:
            Utils.debug("DEBUG: %s" % data.decode("utf-8"))