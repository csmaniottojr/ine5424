from enum import Enum

class CommandMessageType(Enum):
    UNKNOWN_MESSAGE                 = 0
    COMMAND_READ_REQUEST            = 1
    COMMAND_READ_RESPONSE           = 2
    COMMAND_WRITE_REQUEST           = 3
    COMMAND_WRITE_RESPONSE          = 4