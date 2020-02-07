import socket
import json
import serial
from time import sleep

ip = "192.168.18.89"
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((ip, 50000))

ser = serial.Serial("/dev/ttyACM0", 9600)  # change ACM number as found from ls /dev/tty/ACM*
ser.baudrate = 9600

path = [[3000, "L"], [2400, "R"], [5600, "L"], [2000, "R"]]

try:
    while True:
        nachricht = ser.readline()
        if nachricht == "loop_begin":
            s.send(nachricht.encode())
            antwort = s.recv(1024)
            # content = json.loads(antwort.decode())
            content = path
            ser.write("data_ready")
            for i in content:
                strecke = i[0]
                drehung = i[1]
                if ser.readline() == "ready_strecke":
                    ser.write(strecke)
                if ser.readline() == "ready_drehung":
                    ser.write(drehung)
        sleep(1)

finally:
    s.close()

