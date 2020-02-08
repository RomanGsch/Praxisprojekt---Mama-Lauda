import socket
import json
import serial
import threading
from time import sleep


class communication(threading.Thread):
    def __init__(self, path):
        threading.Thread.__init__(self)
        self.deamon = True
        self.path = path
        
        self.cmd = ""
        self.cmdTF = False
    
    def run(self):
        try:
            ser.reset_output_buffer()
            ser.reset_input_buffer()
            ser.write(self.path.encode("ascii"))  # utf-8

            while not self.cmdTF:
                self.cmd = ser.readline().decode("ascii")  # utf-8
                print(self.cmd)
                if str(self.cmd) == "D\r\n":
                    print("True")
                    self.cmdTF = True
                    start_next_session.set()
                    break
                else:
                    print("False")
                    self.cmdTF = False
                    continue
        finally:
            self.cmdTF = False


if __name__ == "__main__":
    # ip = "192.168.18.89"
    # s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # s.connect((ip, 50000))

    ser = serial.Serial("/dev/cu.usbmodem1411", 9600, timeout=2)  # change ACM number as found from ls /dev/tty/ACM*
    # ser.baudrate = 9600

    start_next_session = threading.Event()

    path = ["0000X", "2400R", "5600L", "2000R", "6000L", "7899R", "2988L"]

    for i in path:
        com_thread = communication(i)
        com_thread.run()
        start_next_session.wait()
