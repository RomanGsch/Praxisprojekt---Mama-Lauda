# coding=utf8
import tkinter as tk
from w1thermsensor import W1ThermSensor
import RPi.GPIO as GPIO
import threading
import json
import webbrowser


class Mainframe(tk.Frame):
    """Für den Hauptframe"""

    def __init__(self, start_stop_pin, licht_luft_pin, master, *args, **kwargs):
        # *args packs positional arguments into tuple args
        # **kwargs packs keyword arguments into dict kwargs

        tk.Frame.__init__(self, master, *args, **kwargs)
        # in this case the * an ** operators unpack the parameters

        # Pins
        self.start_stop_pin = start_stop_pin
        self.licht_luft_pin = licht_luft_pin

        # Widgets
        # Variablen
        self.Temperatur = tk.IntVar()
        self.Rauch = tk.IntVar()
        self.Magneto = tk.IntVar()

        # Labels
        tk.Label(self, textvariable=self.Temperatur).pack()
        tk.Label(self, textvariable=self.Rauch).pack()
        tk.Label(self, textvariable=self.Magneto).pack()

        # Buttons
        tk.Button(self, text="Starten", command=self.start_stop).pack()
        tk.Button(self, text="Stop", command=self.start_stop).pack()
        tk.Button(self, text="Licht Ein", command=self.licht_luft_ein).pack()
        tk.Button(self, text="Licht Aus", command=self.licht_luft_aus).pack()
        self.TimerInterval = 500  # millisec
        
        # Variablen für Get_Sens_Data
        self.temp = 0
        self.rauch = 0
        self.winkel = 0
        
        # call Get Temp which will call itself after a delay
        self.get_data()
        
    def get_data(self):
        self.Temperatur.set(self.temp)
        self.Rauch.set(self.rauch)
        self.Magneto.set(self.winkel)
        
        try:
            file = open("/home/pi/Desktop/unfall_data_temp.json")
            content_file = file.read()
            content = json.loads(content_file)
            self.temp_wert = content["TemperaturSensor"]["Temperatur"]
            self.temp = "Temperatur: {}°C".format(self.temp_wert)
            file.close()

            if self.temp_wert > 30:
                self.start_stop()
                print("zu heiß, stop...")

            print(self.temp)

        except Exception as e:
            print("Schon offen [Temperatur]: {}".format(e))
            
        try: 
            file = open("/home/pi/Desktop/unfall_data_rauch.json")
            content_file = file.read()
            content = json.loads(content_file)
            self.rauch_wert = content["RauchSensor"]["Rauch"]
            self.rauch = "Rauch: {}ppm".format(self.rauch_wert)
            file.close()

            if self.rauch_wert > 1.5:
                self.licht_luft_ein()

            print(self.rauch)

        except Exception as e:
            print("Schon offen [Rauch]: {}".format(e))
        
        try:
            file = open("/home/pi/Desktop/unfall_data_magneto.json")
            content_file = file.read()
            content = json.loads(content_file)
            self.winkel_wert = content["MagnetoSensor"]["Winkel"]
            self.winkel = "Winkel: {}°".format(self.winkel_wert)
            file.close()

            print(self.winkel)
        except Exception as e:
            print("Schon offen [Winkel]: {}".format(e))

        self.after(self.TimerInterval, self.get_data)

    def licht_luft_aus(self):
        GPIO.output(self.licht_luft_pin, GPIO.LOW)
        print("licht aus geschalten...")

    def licht_luft_ein(self):
        GPIO.output(self.licht_luft_pin, GPIO.HIGH)
        print("licht ein geschalten...")

    def start_stop(self):
        if GPIO.input(self.start_stop_pin) == GPIO.LOW:
            GPIO.output(self.start_stop_pin, GPIO.HIGH)
            print("motor geschalten...")
        else:
            GPIO.output(self.start_stop_pin, GPIO.LOW)


class App(tk.Tk):
    def __init__(self, licht_luft_pin, start_stop_pin):
        tk.Tk.__init__(self)
        self.licht_luft_pin = licht_luft_pin
        self.start_stop_pin = start_stop_pin

        w = 320  # 1280  # width for the Tk root
        h = 720  # 720  # height for the Tk root
        x = 0
        y = 0

        self.title('Mama Lauda')
        self.geometry('%dx%d+%d+%d' % (w, h, x, y))
        Mainframe(self, self.licht_luft_pin, self.start_stop_pin).pack()

        self.mainloop()


if __name__ == '__main__':
    licht_pin = 27
    start_pin = 17
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(licht_pin, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(start_pin, GPIO.OUT, initial=GPIO.LOW)
    
    webbrowser.open("https://192.168.18.80:8000")
    
    App(licht_pin, start_pin)
