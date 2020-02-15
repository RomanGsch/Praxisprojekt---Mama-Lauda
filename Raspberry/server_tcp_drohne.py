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

        if x_y0[1] == x_y1[1]:#in range(y1_rangeM, y1_rangeP):
            strecke = x_y1[0] - x_y0[0]
            if strecke > 0:
                #drehung = "R"
                if y_2 < x_y1[1]:#not in range(y1_rangeM, y1_rangeP):
                    drehung = "L"  # kann auch R sein wegen versch. X/Y
                else:
                    drehung = "R"  # kann auch R sein wegen versch. X/Y
            else:
                #drehung = "L"
                if y_2 < x_y1[1]:#not in range(y1_rangeM, y1_rangeP):
                    drehung = "R"  # kann auch R sein wegen versch. X/Y
                else:
                    drehung = "L"  # kann auch R sein wegen versch. X/Y

        else:
            strecke = x_y1[1] - x_y0[1]
            if strecke > 0:
                #drehung = "L"
                if x_2 < x_y1[0]:#not in range(x1_rangeM, x1_rangeP):
                    drehung = "R"  # kann auch R sein wegen versch. X/Y
                else:
                    drehung = "L"  # kann auch L sein wegen versch. X/Y
            else:
                #drehung = "R"
                if x_2 < x_y1[0]:#not in range(x1_rangeM, x1_rangeP):
                    drehung = "L"  # kann auch L sein wegen versch. X/Y
                else:
                    drehung = "R"  # kann auch R sein wegen versch. X/Y
                    
        strecke = int(abs(strecke))*42
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


    liste_koordinaten = [
        [0.0, 0.0], [75.0, 0.0], [75.0, 60.0], [45.0, 60.0], [45.0, 90.0],
        [15.0, 90.0], [15.0, 120.0], [75.0, 120.0], [105.0, 120.0],
        [105.0, 90.0], [135.0, 90.0], [135.0, 120.0], [165.0, 120.0],
        [165.0, 90.0], [195.0, 90.0], [195.0, 150.0]]
    
    path = prep_koord(liste_koordinaten)
    print(path)
    
    # path = ['0000X', '1260L']

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
