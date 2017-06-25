from .register_serialization import RegisterSerialization
from .register_message_type import RegisterMessageType

class RegisterMessageResponser(object):

    def __init__(self, model_controller):
        self.model_controller = model_controller

    def response(self, msg):
        barray = bytearray()

        barray.append(int(msg[0]))
        barray.append(RegisterSerialization.BASE_SIZE)
        barray.extend(msg[2:6])

        msg_type = RegisterSerialization.deserialize_message_type(msg)
        print("msg type: ", msg_type)

        if msg_type == RegisterMessageType.REGISTER_REQUEST:
            id = int.from_bytes(emote_id, byteorder=sys.byteorder)
            barray.append(RegisterMessageType.REGISTER_RESPONSE.value)
            
            #TODO dar um jeito nisso
            #barray.append(int(self.model_controller.check_if_exists(id)))
            barray.append(0x00)
            barray[1] = int(barray[1]) + 1

        elif msg_type == RegisterMessageType.REGISTER_OBJECT_REQUEST:
            barray.append(RegisterMessageType.REGISTER_OBJECT_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_SERVICE_REQUEST:
            barray.append(RegisterMessageType.REGISTER_SERVICE_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_PARAMETER_REQUEST:
            barray.append(RegisterMessageType.REGISTER_PARAMETER_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_OPTION_REQUEST:
            barray.append(RegisterMessageType.REGISTER_OPTION_RESPONSE.value)

        elif msg_type == RegisterMessageType.REGISTER_END_OBJECT_REQUEST:
            barray.append(RegisterMessageType.REGISTER_END_OBJECT_RESPONSE.value)
        
        print('resposta: ', ([ "0x%02x" % b for b in barray ]))
        return barray
