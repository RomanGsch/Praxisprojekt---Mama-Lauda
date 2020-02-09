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
        self.temp_degrees = 0
        self.degrees1 = 0
        self.degrees2 = 0

    def get_degrees(self):
        try:
            file = open("/home/pi/Desktop/unfall_data_magneto.json")
            content_file = file.read()
            content = json.loads(content_file)
            self.temp_degrees = content["MagnetoSensor"]["Winkel"]
            file.close()
        except Exception as e:
            print("Deg_Error: {}".format(e))

        return self.temp_degrees
    
    def run(self):
        try:
            ser.reset_output_buffer()
            ser.reset_input_buffer()
            ser.write(self.path.encode("ascii"))  # utf-8
            
            while not self.cmdTF:
                self.cmd = ser.readline().decode("ascii")  # utf-8
                print(self.cmd)

                # eventuell nicht mehr benötigt wenn mit drehung ist
                if str(self.cmd) == "D\r\n":  # D for Done # eventuell kein str() benötigt
                    print("True")
                    self.cmdTF = True
                    start_next_session.set()
                    break
                
                elif str(self.cmd) is "L\r\n" or str(self.cmd) is "R\r\n":
                    self.degrees1 = self.get_degrees()
                    deg_calc = 0
                        
                    while deg_calc < 90:
                        # ser.reset_output_buffer()  # eventuell nicht benötigt
                        ser.write("0000T".encode("ascii"))
                        print("turning: {}".format(deg_calc))
                        self.degrees2 = self.get_degrees()
                        deg_calc = abs(self.degrees1 - self.degrees2)

                    # ser.reset_output_buffer()  # eventuell nicht benötigt
                    ser.write("0000F".encode("ascii"))
                    print("stop turning")
                    # um diesen teil des path zu beenden
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
