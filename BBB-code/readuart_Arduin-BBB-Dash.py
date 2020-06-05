import Adafruit_BBIO.UART as UART

import serial

import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt


# selecting the uart on which you want to communicate
UART.setup("UART1")

disp = serial.Serial(port = "/dev/ttyO1", baudrate = 9600)

disp.close()
disp.open()

while True:
	client = mqtt.Client()

	client.connect("localhost", 1883, 60)
	# initializing lists for string separation
	chars = []
	charsSpeed = []
	charsVoltIn = []
	charsCurrent = []
	charsConf = []
	case1 = 0
	case2 = 0

	if disp.isOpen():

		print "I'm reading\n"
		line = disp.readline()
		print line
		# getting all data retrieved from uart connection in same list
		for i in line:
			chars.append(i)

		size1 = len(chars)
		i = 0
		while i < size1:
			# getting speed data
			if chars[i] == 'S':
				case1 = 1 #means start of frame
				i = i + 1
				while chars[i] != 'C':
					charsSpeed.append(chars[i])
					i = i + 1
					
			# getting voltage data
			if chars[i] == 'C':
				i = i + 1
				while chars[i] != 'V':
					charsCurrent.append(chars[i])
					i = i + 1

			# getting voltage data
			if chars[i] == 'V':
				i = i + 1
				while chars[i] != '\r':
					charsVoltIn.append(chars[i])
					i = i + 1
			
			# getting confirmation data
			if chars[i] == "l":
				case2 = 1 #means end of frame =>status received
				i = i + 1
				while chars[i] != "\r":
					charsConf.append(chars[i])
					i = i + 1
			i = size1
		
		# publish the different datasets on the right topics through MQTT
		if case1 == 1:
			valueSpeed = int("".join(map(str, charsSpeed)))
			valueVoltIn = int("".join(map(str, charsVoltIn)))
			valueCurrent = int("".join(map(str, charsCurrent)))
			publish.single("sensor/time", valueSpeed, hostname="localhost")
			publish.single("sensor/voltageIn", valueVoltIn, hostname="localhost")
			publish.single("sensor/current", valueCurrent, hostname="localhost")
		if case2 == 1:
			valueConf = ''.join(charsConf)
			print 'publish'
			publish.single("sensor/conf", valueConf, hostname="localhost")


disp.close()

