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
n = ser.write(b':A106AB\r\n')
print(n)
print('\nFechando...\n')
ser.close()
