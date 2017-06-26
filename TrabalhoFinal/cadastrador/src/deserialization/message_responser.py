from deserialization.message_type import MessageType
import sys

class MessageResponser(object):

    def __init__(self, model_controller):
        self.model_controller = model_controller

    def response(self, msg):
        print('msg: ', ([ "0x%02x" % b for b in msg ]))
        barray = bytearray()
        barray.append(int(msg[0]))
        barray.append(7)#BASE_SIZE
        emote_id = msg[2:6]

        barray += emote_id

        msg_type = MessageType(msg[6])
        print("msg type: ", msg_type)

        if msg_type == MessageType.REGISTER_REQUEST:
            id = int.from_bytes(emote_id, byteorder=sys.byteorder)
            barray.append(MessageType.REGISTER_RESPONSE.value)
            barray.append(int(self.model_controller.check_if_exists(id)))
            barray[1] = int(barray[1]) + 1

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
        
        print('resposta: ', ([ "0x%02x" % b for b in barray ]))
        return barray
