import paho.mqtt.client as paho

import serial

import time

mqttc = paho.Client()


# Settings for connection

host = "localhost"

topic= "Mbed"

port = 1883


# Callbacks

def on_connect(self, mosq, obj, rc):

    print("Connected rc: " + str(rc))


def on_message(mosq, obj, msg):

    print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n");


def on_subscribe(mosq, obj, mid, granted_qos):

    print("Subscribed OK")


def on_unsubscribe(mosq, obj, mid, granted_qos):

    print("Unsubscribed OK")



# Set callbacks

mqttc.on_message = on_message

mqttc.on_connect = on_connect

mqttc.on_subscribe = on_subscribe

mqttc.on_unsubscribe = on_unsubscribe


# Connect and subscribe

print("Connecting to " + host + "/" + topic)

mqttc.connect(host, port=1883, keepalive=60)

mqttc.subscribe(topic, 0)

####################################################
# XBee setting

serdev = '/dev/ttyUSB0'

s = serial.Serial(serdev, 9600)


s.write("+++".encode())

char = s.read(2)

print("Enter AT mode.")

print(char.decode())


s.write("ATMY 0x140\r\n".encode())

char = s.read(3)

print("Set MY 0x140.")

print(char.decode())


s.write("ATDL 0x240\r\n".encode())

char = s.read(3)

print("Set DL 0x240.")

print(char.decode())


s.write("ATID 0x1\r\n".encode())

char = s.read(3)

print("Set PAN ID 0x1.")

print(char.decode())


s.write("ATWR\r\n".encode())

char = s.read(3)

print("Write config.")

print(char.decode())


s.write("ATMY\r\n".encode())

char = s.read(4)

print("MY :")

print(char.decode())


s.write("ATDL\r\n".encode())

char = s.read(4)

print("DL : ")

print(char.decode())


s.write("ATCN\r\n".encode())

char = s.read(3)

print("Exit AT mode.")

print(char.decode())


print("start logging")
####################################################


def readchar():
    acc = ""
    read = 1
    while read:

        while(s.readable()):

            char=s.read(1)

            acc += char.decode()
            
            if (acc == "\r"):
                print("rrr")
            if char.decode() == "\n" or char.decode() == "\r":
                read = 0
                break
    print("acc: "+acc)
    return acc

    
i = 0

while (1):

    if i == 0:
        print("i=0")
        readchar()
        time.sleep(1)
        print("i=0")

    log =readchar()

    print("nnnnn")

    mesg = "log "+str(i)+":"+log

    mqttc.publish(topic, mesg)

    print(mesg)

    i += 1

s.close()