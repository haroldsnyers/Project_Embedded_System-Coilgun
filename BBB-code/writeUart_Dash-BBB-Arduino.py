import Adafruit_BBIO.UART as UART
 
import serial

import paho.mqtt.client as mqtt
import paho.mqtt.subscribe as subscribe

client = mqtt.Client()

# selecting the uart on which you want to communicate
UART.setup("UART1")
 
disp = serial.Serial (port = "/dev/ttyO1", baudrate=9600)
 
disp.close()
disp.open()
 
while True:
        if disp.isOpen():

                print "Serial is Open\n"
                client.connect("172.17.4.73", 1883, 60)
                
                # reading the data coming from the topic it is subscribed to
                msg = subscribe.simple("sensor/voltage", hostname="localhost")
                print("%s %s" % (msg.topic, msg.payload))
                msg = msg.payload
                print(msg)
                # sending only the voltage value
                if msg != "true":
                        disp.write(msg)
                disp.write("\n")

print "Sorry!!! You not able to do communicate with device" 
disp.close()

