import serial, time
import struct
import sys
from IoT.messages import *

#https://docs.python.org/2/tutorial/datastructures.html
#https://docs.python.org/2/tutorial/classes.html
#https://docs.python.org/2/tutorial/modules.html [args, relative]
#https://stackoverflow.com/questions/1057431/loading-all-modules-in-a-folder-in-python
#https://docs.python.org/2/library/struct.html

BYTE_ORDER = ((sys.byteorder == "little") and "<" or ">") 

REGISTER_START_CHAR = ';'
REGISTER_BASE_SIZE = 7
COMMAND_START_CHAR = ':'
COMMAND_BASE_SIZE = 9
MAX_LENGTH = 100 #tem que pegar o MTU...

def isValidMsg(start, size):
    return (((chr(start) == REGISTER_START_CHAR and size >= REGISTER_BASE_SIZE) or
        (chr(start) == COMMAND_START_CHAR and size >= COMMAND_BASE_SIZE)) and
        size <= MAX_LENGTH)

DEVICE='/dev/ttyACM0'
ser = serial.Serial()
ser.port=DEVICE
ser.baudrate=115200
ser.write_timeout=2.0

if not ser.is_open:
    print('Abrindo...')
    try:
        ser.open()
    except Exception as msg:
        print(msg)
        exit()

ser.flushInput()
ser.flushOutput()

print(ser.get_settings())
print('Lendo da porta por 60s...\n')

t_end = time.time() + 60
while time.time() < t_end:
    msg = bytearray()
    length = 0

    ch = ser.read()
    print(ch)
    msg.append(ch[0])
    length += 1

    ch = ser.read()
    print(ch)
    msg.append(ch[0])
    length += 1
    if not isValidMsg(msg[0], msg[1]):
        print("Entrada invalida!\n")
        continue
    print(msg)
    while length < msg[1]:
        ch = ser.read()
        msg.append(ch[0])
        length += 1

    print([ "0x%02x" % b for b in msg ])
    message = register_serialization.RegisterSerialization.deserialize(msg)
    print("Deserialized size: %d" % message.get_size())
    print("Deserialized ID: %d" % message.get_id())
    print("Deserialized Type: %d" % message.get_type())
    print("Deserialized Param Name: %s" % message.get_parameter_name())
    print("Deserialized min: %s" % [ "0x%02x" % b for b in message.get_min_value() ])
    min = struct.unpack((BYTE_ORDER+"f"), message.get_min_value()[1:5])[0]
    print("   min: %f" % min)
    print("Deserialized max: %s" % [ "0x%02x" % b for b in message.get_max_value() ])
    max = struct.unpack((BYTE_ORDER+"f"), message.get_max_value()[1:5])[0]
    print("   min: %f" % max)
    print("\n")
print('\nFechando...\n')
ser.close()
