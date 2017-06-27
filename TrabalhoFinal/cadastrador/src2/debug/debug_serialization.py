class DebugSerialization(object):

    # Formato das mensagens de Debug:
    # Byte 0            1       2           x     x+2           
    #      +------------+-------+---- ~ ----+------+
    #      | START_CHAR | space | debug msg | \r\n |
    #      +------------+-------+---- ~ ----+------+

    START_CHAR = '#'

    @staticmethod
    def is_valid_message(message, length):
        return chr(message[0]) == DebugSerialization.START_CHAR and \
            chr(message[-2]) == '\r' and chr(message[-1]) == '\n'