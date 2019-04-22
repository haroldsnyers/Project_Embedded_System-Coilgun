import Adafruit_BBIO.UART as UART
 
import serial

import paho.mqtt.client as mqtt
import paho.mqtt.subscribe as subscribe

client = mqtt.Client()

UART.setup("UART1")
 
disp = serial.Serial (port = "/dev/ttyO1", baudrate=9600)
 
disp.close()
disp.open()
 
while True:
        if disp.isOpen():

                print "Serial is Open\n"
                client.connect("localhost", 1883, 60)

                msg = subscribe.simple("sensor/test", hostname="localhost")
                print("%s %s" % (msg.topic, msg.payload))
                msg = msg.payload
                print(msg)
                if msg != "true":
                        disp.write(msg)
                disp.write("\n")

print "Sorry!!! You not able to do communicate with device" 
disp.close()

