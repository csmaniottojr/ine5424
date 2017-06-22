import serial, time

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
print('Lendo da porta por 60s...')

t_end = time.time() + 60
while time.time() < t_end:
	line = ser.readline().decode('utf-8')
	if len(line) > 2:
		print (line)
print('\nFechando...\n')
ser.close()
