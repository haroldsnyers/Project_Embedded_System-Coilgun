import Adafruit_BBIO.UART as UART

import serial

import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt


UART.setup("UART1")

disp = serial.Serial(port = "/dev/ttyO1", baudrate = 9600)

disp.close()
disp.open()

while True:
	client = mqtt.Client()

	client.connect("localhost", 1883, 60)
	chars = []
	charsSpeed = []
	charsOutput = []
	charsConf = []
	case1 = 0
	case2 = 0

	if disp.isOpen():

		print "I'm reading\n"
		line = disp.readline()
		print line
		for i in line:
			chars.append(i)

		size1 = len(chars)
		i = 0
		while i < size1:
			if chars[i] == 'S':
				case1 = 1
				i = i + 1
				while chars[i] != 'O':
					charsSpeed.append(chars[i])
					i = i + 1

			if chars[i] == 'O':
				i = i + 1
				while chars[i] != '\r':
					charsOutput.append(chars[i])
					i = i + 1

			if chars[i] == "C":
				case2 = 1
				i = i + 1
				while chars[i] != "\r":
					charsConf.append(chars[i])
					i = i + 1
			i = size1

		if case1 == 1:
			valueSpeed = int("".join(map(str, charsSpeed)))
			valueOutput = int("".join(map(str, charsOutput)))
			publish.single("sensor/time", valueSpeed, hostname="localhost")
			publish.single("sensor/output", valueOutput, hostname="localhost")
		if case2 == 1:
			valueConf = ''.join(charsConf)
			print 'publish'
			publish.single("sensor/conf", valueConf, hostname="localhost")


disp.close()


