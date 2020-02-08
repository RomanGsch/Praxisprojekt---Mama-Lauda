import serial
from time import sleep

ser = serial.Serial("/dev/ttyACM1", 9600)  # change ACM number as found from ls /dev/tty/ACM*
ser.baudrate = 9600

while True:
    #read_ser = ser.readline()
    text = "hallo"
    write_ser = ser.write(text.encode())
    #print(read_ser)
    #if read_ser == "Hello From Arduino!":
    #    print("bekommen")
    #else:
    #    print("not")
    sleep(1)
