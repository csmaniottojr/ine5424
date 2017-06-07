import serial

ser = serial.Serial('/dev/ttyACM0', 115200)

while True:
	line = ser.readline().decode('utf-8')
	print (line)
	if line[0] == '0':
		#faca algum tratamento
		ser.write(b'OK 0\n')

	if line[0] == '1':
		#faca algum tratamento
		ser.write(b'OK 1\n')

"""
para executar:
python3 epos_usb.py
"""
 