from .command_message import *
from ..utils.utils import Utils

class CommandSerialization:

    # Formato das mensagens de Command:
    # Bit 0            1          2       6          7        x   
    #     +------------+----------+-------+----------+--- ~ --+
    #     | START_CHAR | msg size | SO id | msg type |  data  |
    #     +------------+----------+-------+----------+--- ~ --+

    START_CHAR = CommandMessage.START_CHAR
    BASE_SIZE = CommandMessage.BASE_SIZE

    @staticmethod
    def serialize(message):
        result = bytearray()

        result.append(ord(CommandSerialization.START_CHAR))

        result.append(message.get_size())

        id = Utils.pack_with_byte_order("I", message.get_id())
        result.extend(b for b in id)

        type = message.get_type()
        tmp = Utils.pack_with_byte_order("B", type.value)
        result.extend(tmp)

        regId = Utils.pack_with_byte_order("H", message.get_register_id())
        result.extend(b for b in regId)

        length = message.get_data_length()
        if (length > 0 and (type == CommandMessageType.COMMAND_WRITE_REQUEST or 
            type == CommandMessageType.COMMAND_READ_RESPONSE)):
            result.extend(b for b in message.get_data())
        
        return result

    @staticmethod
    def deserialize(message):
        result = CommandMessage()

        if chr(message[0]) != CommandSerialization.START_CHAR:
            return result
        
        message = message[1:]#ignora o START_CHAR
        i = 0

        size = int(message[i])
        result.set_size(size)
        i += 1

        id = Utils.unpack_with_byte_order("I", message[i:(i+4)])
        result.set_id(id)
        i += 4

        type = int(message[i])
        result.set_type(type)
        type = result.get_type()
        i += 1

        regId = Utils.unpack_with_byte_order("H", message[i:(i+2)])
        result.set_register_id(regId)
        i += 2

        length = result.get_data_length()
        if (length > 0 and (type == CommandMessageType.COMMAND_WRITE_REQUEST or 
            type == CommandMessageType.COMMAND_READ_RESPONSE)):
            result.set_data((b for b in message[i:(i+length)]), length)
            i += length
        
        return result