import serial
from deserialization.message_validator import MessageValidator
from deserialization.message_deserializer import MessageDeserializer
from deserialization.message_responser import MessageResponser

class SerialPortManager(object):

    def __init__(self, port, baudrate, model_controller):
        self.port = port
        self.baudrate = baudrate
        self.model_controller = model_controller
        self.msg_responser = MessageResponser(model_controller)


    def start(self):
        ser = serial.Serial(self.port, self.baudrate)

        while True:

            lines = []

            while not self.is_last_line(lines):

                line = bytearray()

                while not self.is_end_of_line(line):
                    _byte = ser.read()
                    print (_byte)
                    line += bytearray(_byte)
                else:
                    if chr(line[0]) == '#':
                        print(line)
                    else:
                        n = ser.write(self.msg_responser.response(line))
                        print('escritos %d bytes!' % n)
                        lines.append(line)
            else:
                self.process_lines(lines)


    def is_end_of_line(self, line):
        msg_validator = MessageValidator()
        return msg_validator.check_if_message_is_complete(line)


    def is_last_line(self, lines):
        if len(lines) == 0:
            return False

        msg_validator = MessageValidator()
        return msg_validator.check_if_is_end_message(lines)


    def process_lines(self, lines):
        msg_deserializer = MessageDeserializer(self.model_controller)
        msg_deserializer.deserialize(lines)
