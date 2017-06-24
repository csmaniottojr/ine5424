from deserialization.message_type import MessageType

class MessageValidator(object):

    def check_if_message_is_complete(self, barray):
        size = barray[1]
        return size == len(barray)


    def check_if_is_end_message(self, barray_list):
        last_barray = barray_list[-1]

        msg_type = last_barray[6]

        msg_type = MessageType(msg_type)
        return msg_type in MessageType.end_messages()
