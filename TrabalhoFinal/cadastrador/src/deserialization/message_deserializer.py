import struct
import sys
from deserialization.message_type import MessageType
from deserialization.parameter_type import ParameterType
from models import SmartObject, Service, ParameterBoolean, ParameterFloat,\
    ParameterInteger, ParameterOption, Option

class MessageDeserializer(object):

    def __init__(self, model_controller):
        self.model_controller = model_controller


    def deserialize(self, lines):
        first_barray = lines[0]
        size = first_barray[1]
        emote_id = int.from_bytes(first_barray[2:6], byteorder=sys.byteorder)
        first_msg_type = MessageType(first_barray[6])

        if first_msg_type == MessageType.REGISTER_REQUEST:
            print ('exists: ', self.model_controller.check_if_exists(emote_id))
            return self.model_controller.check_if_exists(emote_id)

        elif first_msg_type == MessageType.REGISTER_OBJECT_REQUEST:

            so_name = first_barray[7:size].decode('utf-8')
            smart_object = SmartObject(so_name, emote_id)

            print('so name: ', so_name)

            i_line = 0
            while i_line < len(lines) - 1:
                i_line += 1
                msg_type = MessageType(lines[i_line][6])

                if msg_type == MessageType.REGISTER_SERVICE_REQUEST:
                    size = lines[i_line][1]
                    service_name = lines[i_line][7:size].decode('utf-8')
                    service = Service(service_name)

                    while MessageType(lines[i_line + 1][6]) == MessageType.REGISTER_PARAMETER_REQUEST:
                        i_line += 1
                        param_type = ParameterType(lines[i_line][7])
                        size = lines[i_line][1]
                        param = None
                        if param_type == ParameterType.INTEGER:
                            reg_id = int.from_bytes(lines[i_line][8:10], byteorder=sys.byteorder)
                            min_value = int.from_bytes(lines[i_line][10:14], byteorder=sys.byteorder)
                            max_value = int.from_bytes(lines[i_line][14:18], byteorder=sys.byteorder)
                            param_name = lines[i_line][18:size].decode('utf-8')
                            param = ParameterInteger(param_name, reg_id, min_value, max_value)

                        elif param_type == ParameterType.FLOAT:
                            reg_id = int.from_bytes(lines[i_line][8:10], byteorder=sys.byteorder)
                            min_value = struct.unpack('f', lines[i_line][10:14])[0]
                            max_value = struct.unpack('f', lines[i_line][14:18])[0]
                            param_name = lines[i_line][18:size].decode('utf-8')
                            param = ParameterFloat(param_name, reg_id, min_value, max_value)

                        elif param_type == ParameterType.BOOLEAN:
                            reg_id = int.from_bytes(lines[i_line][8:10], byteorder=sys.byteorder)
                            param_name = lines[i_line][12:size].decode('utf-8')
                            param = ParameterBoolean(param_name, reg_id)

                        elif param_type == ParameterType.COMBO:
                            reg_id = int.from_bytes(lines[i_line][8:10], byteorder=sys.byteorder)
                            param_name = lines[i_line][10:size].decode('utf-8')
                            param = ParameterOption(param_name, reg_id)

                            while MessageType(lines[i_line + 1][6]) == MessageType.REGISTER_OPTION_REQUEST:
                                i_line += 1
                                size = lines[i_line][1]
                                op_name = lines[i_line][8:size].decode('utf-8')
                                option = Option(op_name)
                                param.add_option(option)

                        service.add_parameter(param)
                    smart_object.add_service(service)

                elif msg_type == MessageType.REGISTER_END_OBJECT_REQUEST:
                    self.model_controller.save(smart_object)
