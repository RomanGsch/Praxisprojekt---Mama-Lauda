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
        self.degrees1 = 0
        self.degrees2 = 0
    
    def run(self):
        try:
            ser.reset_output_buffer()
            ser.reset_input_buffer()
            ser.write(self.path.encode("ascii"))  # utf-8
            
            while not self.cmdTF:
                self.cmd = ser.readline().decode("ascii")  # utf-8
                print(self.cmd)
                if str(self.cmd) == "D\r\n":  # D for Done
                    print("True")
                    self.cmdTF = True
                    start_next_session.set()
                    break
                
                elif str(self.cmd) is "L\r\n":# or "L\r\n"):
                    try:
                        file = open("/home/pi/Desktop/unfall_data_magneto.json")
                        content_file = file.read()
                        content = json.loads(content_file)
                        self.degrees1 = content["MagnetoSensor"]["Winkel"]
                        file.close()
                    except Exception as e:
                        print("Deg1_Error: {}".format(e))
                        
                    deg_calc = 0                        
                        
                    while deg_calc < 60:
                        ser.reset_output_buffer()
                        print(deg_calc)
                        ser.write("T".encode("ascii"))
                        print("turning: {}".format(deg_calc))
                        print(ser.readline())
                        try:
                            file = open("/home/pi/Desktop/unfall_data_magneto.json")
                            content_file = file.read()
                            content = json.loads(content_file)
                            self.degrees2 = content["MagnetoSensor"]["Winkel"]
                            file.close()
                        except Exception as e:
                            print("Deg2_Error: {}".format(e))
                        
                        deg_calc = abs(self.degrees1 - self.degrees2)
                        sleep(0.1)
                        # deg_calc = 100

                    ser.reset_output_buffer()
                    ser.write(str(0).encode("ascii"))
                    sleep(0.1)
                    print("stop turning")
                    print(ser.readline())
                    self.cmdTF = True
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
    
    try:
        ser = serial.Serial("/dev/ttyACM0", 9600, timeout=2)  # change ACM number as found from ls /dev/tty/ACM* /dev/cu.usbmodem1411 for mac
    except:
        ser = serial.Serial("/dev/ttyACM1", 9600, timeout=2)
        
    start_next_session = threading.Event()

    path = ["0000X", "2000L", "2000L", "2000R", "2000L", "2000R", "2000L"]

    for i in path:
        com_thread = communication(i)
        com_thread.run()
        start_next_session.wait()
