# coding=utf8
from subprocess import Popen
from time import sleep

try:
    Popen(["python3", "/home/pi/PyProjects/Praxisprojekt---Mama-Lauda/Raspberry/get_sens_data.py"])
    print("get_sens_data.py gestartet")
except Exception as e:
    print("fehler beim starten: get_sens_data.py: {}".format(e))
finally:
    sleep(2)

try:
    Popen(["python3", "/home/pi/PyProjects/Praxisprojekt---Mama-Lauda/Raspberry/cam_server.py"])
    print("cam_server.py gestartet")
except Exception as e:
    print("fehler beim starten: cam_server.py: {}".format(e))
finally:
    sleep(2)

try:
    Popen(["python3", "/home/pi/PyProjects/Praxisprojekt---Mama-Lauda/Raspberry/gui.py"])
    print("gui.py gestartet")
except Exception as e:
    print("fehler beim starten: gui.py: {}".format(e))
finally:
    sleep(2)
