import serial, time
import struct
import sys

byte_order = sys.byteorder

REGISTER_START_CHAR = ';'
REGISTER_BASE_SIZE = 6
COMMAND_START_CHAR = ':'
COMMAND_BASE_SIZE = 8
MAX_LENGHT = 100 #tem que pegar o MTU...

def isValidMsg(start, size):
    return (((chr(start) == REGISTER_START_CHAR and size >= REGISTER_BASE_SIZE) or
        (chr(start) == COMMAND_START_CHAR and size >= COMMAND_BASE_SIZE)) and
        size <= MAX_LENGHT)

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
    len = 0

    ch = ser.read()
    print(ch)
    msg.append(ch[0])
    len += 1

    ch = ser.read()
    print(ch)
    msg.append(ch[0])
    len += 1
    if not isValidMsg(msg[0], msg[1]):
        print("Entrada invalida!\n")
        continue
    print(msg)
    while len <= msg[1]:
        ch = ser.read()
        msg.append(ch[0])
        len += 1

    print(msg)
    size = msg[1]
    emote_id = int.from_bytes(msg[2:6], byteorder=byte_order)
    msg_type = msg[6]
    print("size: %d - ID: %d - type: %d" % (size, emote_id, msg_type))
    print("\n")
print('\nFechando...\n')
ser.close()
