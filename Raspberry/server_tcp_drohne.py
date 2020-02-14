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
            print(self.path)
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

        y1_rangeM = int(x_y1[1]) - 5
        y1_rangeP = int(x_y1[1]) + 5
        x1_rangeM = int(x_y1[0]) - 5
        x1_rangeP = int(x_y0[0]) + 5

        if x_y0[1] in range(y1_rangeM, y1_rangeP):
            strecke = x_y1[0] - x_y0[0]
            if strecke > 0:
                if y_2 not in range(y1_rangeM, y1_rangeP):
                    drehung = "R"  # kann auch R sein wegen versch. X/Y
                else:
                    drehung = "L"  # kann auch R sein wegen versch. X/Y
            else:
                if y_2 not in range(y1_rangeM, y1_rangeP):
                    drehung = "L"  # kann auch R sein wegen versch. X/Y
                else:
                    drehung = "R"  # kann auch R sein wegen versch. X/Y

        else:
            strecke = x_y1[1] - x_y0[1]
            if strecke < 0:
                if x_2 not in range(x1_rangeM, x1_rangeP):
                    drehung = "L"  # kann auch R sein wegen versch. X/Y
                else:
                    drehung = "R"  # kann auch L sein wegen versch. X/Y
            else:
                if x_2 not in range(x1_rangeM, x1_rangeP):
                    drehung = "R"  # kann auch L sein wegen versch. X/Y
                else:
                    drehung = "L"  # kann auch R sein wegen versch. X/Y
        strecke = int(abs(strecke))
        streckeSTR = str(strecke)

        while len(streckeSTR) < 4:
            streckeSTR = "0" + streckeSTR

        koordinate_neu = streckeSTR + drehung
        liste_koordinaten_neu.append(koordinate_neu)

    else:
        print("path ready...")

    liste_koordinaten_neu.insert(0, "0000X")

    return liste_koordinaten_neu


if __name__ == "__main__":
    liste_koordinaten = []

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind(("", 50000))
    s.listen(1)
    try:
        while True:
            komm, addr = s.accept()
            while True:
                data = komm.recv(1024)
                if not data:
                    komm.close()
                    break
                print("[{}] {}".format(addr[0], data.decode()))
                path_content = data
                liste_koordinaten = json.loads(path_content)
                if liste_koordinaten is not []:
                    break
            break

    finally:
        s.close()
    # liste_koordinaten = [[63.0, 27.0], [69.0, 31.0], [181.0, 28.0], [184.0, 67.0], [114.0, 73.0], [150.0, 146.0], [155.0, 110.0], [157.0, 109.0], [191.0, 113.0], [192.0, 186.0], [121.0, 193.0], [122.0, 229.0], [120.0, 262.0], [89.0, 265.0], [98.0, 291.0], [213.0, 283.0]]

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
