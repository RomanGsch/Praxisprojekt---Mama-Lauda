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
    
    def run(self):
        try:
            while True:
                # nachricht = "loop_begin"
                # s.send(nachricht.encode())
                # antwort = s.recv(1024)
                # content = json.loads(antwort.decode())
        
                content = self.path
                ser.reset_output_buffer()
                ser.reset_input_buffer()
        
                for i in content:
                    ser.write(i.encode("ascii"))  # utf-8
                    # ret = ser.readline().decode("ascii")
                    # while ret is not "":
                    #     text = ser.readline().decode("ascii")
                    #     print("Return: " + text)
                    #     if text is "":
                    #         break
                    # print(ret)
                    self.cmd = ser.readline().decode("ascii")  # utf-8
                    if self.cmd == "D":
                        self.cmd = True
                    else:
                        self.cmd = False
                while self.cmd:
                    # ser.reset_input_buffer()
                    self.cmd = ser.readline().decode("ascii")
                    print(self.cmd)
                    if str(self.cmd) is "D":
                        break
                    else:
                        print("waiting...")
        
        finally:
            # s.close()
            pass


if __name__ == "__main__":
    # ip = "192.168.18.89"
    # s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # s.connect((ip, 50000))

    ser = serial.Serial("/dev/cu.usbmodem1411", 9600, timeout=2)  # change ACM number as found from ls /dev/tty/ACM*
    # ser.baudrate = 9600

    path = ["0000X", "2400R", "5600L", "2000R", "6000L", "7899R", "2988L"]
    threads = []
    com_thread = communication(path)
    com_thread.run()
