import serial

DEVICE='/dev/ttyACM0'
ser = serial.Serial(
	port=DEVICE,
	baudrate=115200,
	timeout=1.0,
	write_timeout=1.0)

if not ser.is_open:
	#ser.close()
	print('Abrindo...')
	ser.open()

ser.flushInput()
ser.flushOutput()

print(ser.get_settings())
print('Escrevendo...')

#3a a a b0 d 6 3 1 0 1 [ligar o led]
msg = bytearray([0x3a, 0x0a, 0x0a, 0xb0, 0x0d, 0x06, 0x03, 0x01, 0x00, 0x00])
n = ser.write(msg)
print('Escritos %d bytes...' % n)
print('\nFechando...\n')
ser.close()
