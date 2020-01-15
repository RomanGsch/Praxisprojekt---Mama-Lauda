#!/usr/bin/python

import smbus
import time
# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)

#This is the adress we setup in the Arduino Program
adress = 0x04

def writeNumber(value):
    bus.write_byte(adress, value)
    #bus.write_byte_data(adress, 0, value)
    return -1

def readNumber():
    number = bus.read_byte(adress)
    # number = bus.read_byte_data(adress,1)
    return number

while True:
    var= input("Enter 1-9")
    if not var:
        continue

    writeNumber(int(var))
    print ("RPI: Hi Arduino, I  sent you", var)
    # sleep one second
    time.sleep(1)

    number=readNumber()
    print ("Arduino, I received a digit", number)
    
