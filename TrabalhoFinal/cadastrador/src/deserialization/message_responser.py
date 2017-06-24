from deserialization.message_type import MessageType
import sys

class MessageResponser(object):

    def __init__(self, model_controller):
        self.model_controller = model_controller

    def response(self, msg):
        barray = bytearray()
        barray.append(ord(msg[0]))
        barray.append(msg[1])
        emote_id = msg[2:6]

        barray += bytearray(emote_id.to_bytes(4, byteorder=sys.byteorder, signed=True))

        msg_type = MessageType(msg[6])

        if msg_type == MessageType.REGISTER_REQUEST:
            barray.append(MessageType.REGISTER_RESPONSE.value)
            barray.append(int(self.model_controller.check_if_exists(emote_id)))

        elif msg_type == MessageType.REGISTER_OBJECT_REQUEST:
            barray.append(MessageType.REGISTER_OBJECT_RESPONSE.value)

        elif msg_type == MessageType.REGISTER_SERVICE_REQUEST:
            barray.append(MessageType.REGISTER_SERVICE_RESPONSE.value)

        elif msg_type == MessageType.REGISTER_PARAMETER_REQUEST:
            barray.append(MessageType.REGISTER_PARAMETER_RESPONSE.value)

        elif msg_type == MessageType.REGISTER_OPTION_REQUEST:
            barray.append(MessageType.REGISTER_OPTION_RESPONSE.value)

        elif msg_type == MessageType.REGISTER_END_OBJECT_REQUEST:
            barray.append(MessageType.REGISTER_END_OBJECT_RESPONSE.value)

        return barray
