# coding=utf8
import RPi.GPIO as GPIO
import threading
import py_qmc5883l
import w1thermsensor
from gas_detection import GasDetection
from json import dump
from time import sleep


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
                dump(Temperatur_Data, file)  # json.dump()
                file.close()
            print("Temperatur: {}\n".format(self.temperatur))
            sleep(0.5)


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
            Rauch_Data = {
                "RauchSensor": {
                    "CO": round(ppm[rauch_sens.CO_GAS], 4),
                    "LPG": round(ppm[rauch_sens.LPG_GAS], 4),
                    "Rauch": round(ppm[rauch_sens.SMOKE_GAS], 4)
                    }
                }
            with open("/home/pi/Desktop/unfall_data_rauch.json", 'w') as file:
                dump(Rauch_Data, file)  # json.dump()
                file.close()
            print('CO: {} ppm'.format(ppm[rauch_sens.CO_GAS]))
            print('LPG: {} ppm'.format(ppm[rauch_sens.LPG_GAS]))
            print('Rauch: {} ppm\n'.format(ppm[rauch_sens.SMOKE_GAS]))
            sleep(0.5)


class Magneto(threading.Thread):
    """Um auf Sensorwerte zu kommen"""
    def __init__(self):
        threading.Thread.__init__(self)
        self.deamon = True
        self.temperatur = 0
        
    def run(self):
        magneto_sens = py_qmc5883l.QMC5883L()

        while True:
            messwinkel = int(magneto_sens.get_bearing())
            Magneto_Data = {
                "MagnetoSensor": {
                    "Winkel": messwinkel
                    }
                }

            with open("/home/pi/Desktop/unfall_data_magneto.json", 'w') as file:
                dump(Magneto_Data, file)  # json.dump()
                file.close()
            print("Winkel: {}\n".format(messwinkel))
            sleep(0.25)


class Entfernung(threading.Thread):
    """entfernung messen"""
    def __init__(self, pin_rpm):
        threading.Thread.__init__(self)
        self.deamon = True
        self.temperatur = 0
        self.pin_rpm = pin_rpm

    def run(self):
        #magneto_sens = py_qmc5883l.QMC5883L()
        #startwinkel = int(magneto_sens.get_bearing())
        #minusRange = startwinkel - 30
        #plusRange = startwinkel + 30

        #print("startwinkel = {}".format(startwinkel))
        #print("minusRange = {}".format(minusRange))
        #print("plusRange = {}".format(plusRange))

        # für rpm
        entfernung = 0

        last_state = False
        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self.pin_rpm, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

        while True:
            #messwinkel = int(magneto_sens.get_bearing())
            #if messwinkel in range(minusRange, plusRange):
            current_state = GPIO.input(self.pin_rpm)
            if current_state != last_state:
                entfernung += 0.065*3.14/40
                last_state = current_state
                Entfernung_Data = {
                    "RPMSensor": {
                        "Entfernung": round(entfernung, 3)
                        }
                    }

                with open("/home/pi/Desktop/unfall_data_entfernung.json", 'w') as file:
                    dump(Entfernung_Data, file)  # json.dump()
                    file.close()
                print("Entfernung: {}\n".format(entfernung))    
            #sleep(0.1)

if __name__ == '__main__':
    threads = []
    
    temp_thread = Temperatur()
    rauch_thread = Rauch()
    magneto_thread = Magneto()
    entfernung_thread = Entfernung(pin_rpm=6)
    temp_thread.start()
    rauch_thread.start()
    magneto_thread.start()
    entfernung_thread.start()
    
    #threads.append(temp_thread)
