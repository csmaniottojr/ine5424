# encoding: utf-8
from utils.utils import Utils
from .register_message_type import RegisterMessageType

import struct
import sys
from .parameter_type import ParameterType
from models import SmartObject, Service, ParameterBoolean, ParameterFloat,\
    ParameterInteger, ParameterOption, Option

"""
    Classe responsável pela deserialização das mensagens de registro.
"""
class RegisterSerialization(object):

    def __init__(self, model_controller):
        self.model_controller = model_controller

    # Formato das mensagens de Register:
    # Byte 0            1          2       6          7        x
    #      +------------+----------+-------+----------+--- ~ --+
    #      | START_CHAR | msg size | SO id | msg type |  data  |
    #      +------------+----------+-------+----------+--- ~ --+
    # PS: data se refere aos dados especificos de cada tipo de mensagem

    START_CHAR = ';'
    BASE_SIZE = 7 # start_char + size + id + type

    @staticmethod
    def is_valid_message(message, length):
        return chr(message[0]) == RegisterSerialization.START_CHAR and \
            RegisterSerialization.deserialize_size(message) == length

    @staticmethod
    def deserialize_size(barray):
        return int(barray[1])

    @staticmethod
    def deserialize_emote_id(barray):
        return Utils.unpack_with_byte_order("I", barray[2:6])

    @staticmethod
    def deserialize_message_type(barray):
        return RegisterMessageType(int(barray[6]))

    def deserialize(self, lines):
        first_barray = lines[0]
        size = first_barray[1]
        emote_id = int.from_bytes(first_barray[2:6], byteorder=sys.byteorder)
        first_msg_type = RegisterMessageType(first_barray[6])

        if first_msg_type == RegisterMessageType.REGISTER_REQUEST:
            Utils.debug ('SO %d Exist?: %d\n\n' % (emote_id, \
                self.model_controller.check_if_exists(emote_id)))
            return self.model_controller.check_if_exists(emote_id)

        elif first_msg_type == RegisterMessageType.REGISTER_OBJECT_REQUEST:

            so_name = first_barray[7:size].decode('utf-8')
            smart_object = SmartObject(so_name, emote_id)

            Utils.debug('Salvando SO com nome: %s\n\n' % so_name)

            i_line = 0
            while i_line < len(lines) - 1:
                i_line += 1
                msg_type = RegisterMessageType(lines[i_line][6])

                if msg_type == RegisterMessageType.REGISTER_SERVICE_REQUEST:
                    size = lines[i_line][1]
                    service_name = lines[i_line][7:size].decode('utf-8')
                    service = Service(service_name)

                    while RegisterMessageType(lines[i_line + 1][6]) == RegisterMessageType.REGISTER_PARAMETER_REQUEST:
                        i_line += 1
                        param_type = ParameterType(lines[i_line][7])
                        reg_id = int.from_bytes(lines[i_line][8:10], byteorder=sys.byteorder)
                        read_only = bool(lines[i_line][10])

                        size = lines[i_line][1]
                        param = None
                        if param_type == ParameterType.INTEGER:
                            min_value = int.from_bytes(lines[i_line][11:15], byteorder=sys.byteorder)
                            max_value = int.from_bytes(lines[i_line][15:19], byteorder=sys.byteorder)
                            param_name = lines[i_line][19:size].decode('utf-8')
                            param = ParameterInteger(param_name, reg_id, read_only, min_value, max_value)

                        elif param_type == ParameterType.FLOAT:
                            min_value = struct.unpack('f', lines[i_line][11:15])[0]
                            max_value = struct.unpack('f', lines[i_line][15:19])[0]
                            param_name = lines[i_line][19:size].decode('utf-8')
                            param = ParameterFloat(param_name, reg_id, read_only, min_value, max_value)

                        elif param_type == ParameterType.BOOLEAN:
                            param_name = lines[i_line][13:size].decode('utf-8')
                            param = ParameterBoolean(param_name, reg_id, read_only)

                        elif param_type == ParameterType.COMBO:
                            param_name = lines[i_line][11:size].decode('utf-8')
                            param = ParameterOption(param_name, reg_id, read_only)

                            while RegisterMessageType(lines[i_line + 1][6]) == RegisterMessageType.REGISTER_OPTION_REQUEST:
                                i_line += 1
                                size = lines[i_line][1]
                                op_name = lines[i_line][7:size].decode('utf-8')
                                option = Option(op_name)
                                param.add_option(option)
                        if param is not None:
                            service.add_parameter(param)
                    smart_object.add_service(service)

                elif msg_type == RegisterMessageType.REGISTER_END_OBJECT_REQUEST:
                    self.model_controller.save(smart_object)
