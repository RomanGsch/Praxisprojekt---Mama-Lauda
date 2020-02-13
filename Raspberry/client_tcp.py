import socket
import json
import serial
import threading


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
        """eventuell nicht mehr benötigt, da kein Zugriff auf Magnetometer mehr gemacht wird"""
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


def prep_koord(koordinaten):
    liste_koordinaten_neu = []

    for i in range(0, len(koordinaten)-1):
        if i is not len(koordinaten)-2:
            x_2 = koordinaten[i+2][0]
            y_2 = koordinaten[i+2][1]
        else:
            x_2 = koordinaten[len(koordinaten)-1][0]
            y_2 = koordinaten[len(koordinaten)-1][1]

        x_y0 = koordinaten[i]
        x_y1 = koordinaten[i+1]

        if x_y0[1] == x_y1[1]:
            strecke = x_y1[0] - x_y0[0]
            if strecke > 0:
                if y_2 < x_y1[1]:
                    drehung = "R"
                else:
                    drehung = "L"
            else:
                if y_2 < x_y1[1]:
                    drehung = "L"
                else:
                    drehung = "R"

        else:
            strecke = x_y1[1] - x_y0[1]
            if strecke < 0:
                if x_2 < x_y1[0]:
                    drehung = "R"
                else:
                    drehung = "L"
            else:
                if x_2 < x_y1[0]:
                    drehung = "L"
                else:
                    drehung = "R"

        koordinate_neu = str(abs(strecke)) + drehung
        liste_koordinaten_neu.append(koordinate_neu)

    else:
        print("path ready...")

    liste_koordinaten_neu.insert(0, "0000X")

    return liste_koordinaten_neu


if __name__ == "__main__":
    ip = "192.168.18.75"
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ip, 50000))
    got_path = False
    nachricht = "start"
    liste_koordinaten = []

    # ----------dummie files----------
    # liste_koordinaten = [
    #     [0000, 0000], [4000, 0000], [4000, -4000], [7000, -4000], [7000, 0000], [14000, 0000],
    #     [14000, -6000], [12000, -6000], [12000, -9000], [15000, -9000], [15000, -13000]]
    # path = ["0000X", "2000L", "2000L", "2000R", "2000L", "2000R", "2000L"]

    while not got_path:
        try:
            s.send(nachricht.encode())
            antwort = s.recv(1024)
            print("[{}] {}".format(ip, antwort.decode()))
            path_content = antwort
            liste_koordinaten = json.loads(path_content)
            if liste_koordinaten is not []:
                got_path = True
        finally:
            s.close()

    path = prep_koord(liste_koordinaten)
    print(path)

    try:
        ser = serial.Serial("/dev/ttyACM0", 9600, timeout=2)  # change ACM number as found from ls /dev/tty/ACM* /dev/cu.usbmodem1411 for mac
    except Exception as e:
        print(e)
        ser = serial.Serial("/dev/ttyACM1", 9600, timeout=2)
        
    start_next_session = threading.Event()

    for i in path:
        com_thread = communication(i)
        com_thread.run()
        start_next_session.wait()
