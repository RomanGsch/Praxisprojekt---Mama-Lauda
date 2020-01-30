# coding=utf8

import w1thermsensor
import RPi.GPIO as GPIO
import time
import threading
from gas_detection import GasDetection
import json
import py_qmc5883l


class Temperatur(threading.Thread):
    """Um auf Sensorwerte zu kommen"""
    def __init__(self):
        threading.Thread.__init__(self)
        self.deamon = True
        self.temperatur = 0
        
    def run(self):
        temp_sens = w1thermsensor.W1ThermSensor()
        
        while True:
            self.temperatur = temp_sens.get_temperature()
            Temperatur_Data ={
                "TemperaturSensor":{
                    "Temperatur": self.temperatur
                    }
            }
                
            with open("/home/pi/Desktop/unfall_data_temp.json", 'w') as file:
                json.dump(Temperatur_Data, file)
                file.close()
            print(self.temperatur)
            time.sleep(0.5)
        
class Rauch(threading.Thread):
    """Um auf Sensorwerte zu kommen"""
    def __init__(self):
        threading.Thread.__init__(self)
        self.deamon = True
        self.temperatur = 0
        
    def run(self):
        print('Calibrating ...')
        rauch_sens = GasDetection()
        while True:
            ppm = rauch_sens.percentage()
            print('CO: {} ppm'.format(ppm[rauch_sens.CO_GAS]))
            print('LPG: {} ppm'.format(ppm[rauch_sens.LPG_GAS]))
            print('SMOKE: {} ppm\n'.format(ppm[rauch_sens.SMOKE_GAS]))
            Rauch_Data = {
                "RauchSensor": {
                    "CO": ppm[rauch_sens.CO_GAS],
                    "LPG": ppm[rauch_sens.LPG_GAS],
                    "Rauch": ppm[rauch_sens.SMOKE_GAS]
                    }
                }
            with open("/home/pi/Desktop/unfall_data_rauch.json", 'w') as file:
                json.dump(Rauch_Data, file)
                file.close()
            time.sleep(0.5)

class Magneto(threading.Thread):
    """Um auf Sensorwerte zu kommen"""
    def __init__(self):
        threading.Thread.__init__(self)
        self.deamon = True
        self.temperatur = 0
        
    def run(self):
        magneto_sens = py_qmc5883l.QMC5883L()
        startwinkel = int(magneto_sens.get_bearing())
        minusRange = startwinkel - 30
        plusRange = startwinkel +30

        entfernung = 0

        # print("startwinkel = {}".format(startwinkel))
        # print("minusRange = {}".format(minusRange))
        # print("plusRange = {}".format(plusRange))

        while True:
            messwinkel = int(magneto_sens.get_bearing())
            if messwinkel in range(minusRange, plusRange):
                #hier auswertung der länge
                entfernung += 1
            Magneto_Data = {
                "MagnetoSensor": {
                    "Winkel": messwinkel
                    }
                }
            with open("/home/pi/Desktop/unfall_data_magneto.json", 'w') as file:
                json.dump(Magneto_Data, file)
                file.close()
            print("Winkel: {}".format(messwinkel))
            time.sleep(0.5)


if __name__ == '__main__':
    threads = []
    
    temp_thread = Temperatur()
    rauch_thread = Rauch()
    magneto_thread = Magneto()
    temp_thread.start()
    rauch_thread.start()
    magneto_thread.start()
    
    threads.append(temp_thread)