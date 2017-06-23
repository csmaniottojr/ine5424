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

msg = bytearray([0x3b, 0x07, 0x01, 0x02, 0x03, 0x04, 0x05])
n = ser.write(msg)
print('Escritos %d bytes...' % n)
print('\nFechando...\n')
ser.close()