import tkinter as tk
from w1thermsensor import W1ThermSensor
import RPi.GPIO as GPIO
import threading
import json


class Mainframe(tk.Frame):
    # Mainframe contains the widgets
    # More advanced programs may have multiple frames
    # or possibly a grid of subframes
    
    def __init__(self,master,*args,**kwargs):
        # *args packs positional arguments into tuple args
        # **kwargs packs keyword arguments into dict kwargs
        
        # initialise base class
        tk.Frame.__init__(self,master,*args,**kwargs)
        # in this case the * an ** operators unpack the parameters
        
        # put your widgets here
        self.Temperatur = tk.IntVar()
        self.Rauch = tk.IntVar()
        self.Magneto = tk.IntVar()
        tk.Label(self,textvariable = self.Temperatur).pack()
        tk.Label(self,textvariable = self.Rauch).pack()
        tk.Label(self,textvariable = self.Magneto).pack()
        tk.Button(self, text="Starten", command=self.start_stop).pack()
        tk.Button(self, text="Stop", command=self.start_stop).pack()
        tk.Button(self, text="Licht", command=self.licht_luft).pack()
        self.TimerInterval = 2000  # millisec
        
        # variable for dummy GetTemp
        self.temp = 0
        self.rauch = 0
        self.winkel = 0
        
        # call Get Temp which will call itself after a delay
        self.GetTemp()
        
    def GetTemp(self):
        
        self.Temperatur.set(self.temp)
        self.Rauch.set(self.rauch)
        self.Magneto.set(self.winkel)
        
        
        file = open("/home/pi/Desktop/unfall_data_temp.json")
        content_file = file.read()
        content = json.loads(content_file)
        self.temp = content["TemperaturSensor"]["Temperatur"]
        self.temp = "Temperatur: {}°C".format(self.temp)
        file.close()
        print(self.temp)
        
        file = open("/home/pi/Desktop/unfall_data_rauch.json")
        content_file = file.read()
        content = json.loads(content_file)
        self.rauch = content["RauchSensor"]["Rauch"]
        self.rauch = "Rauch: {}ppm".format(self.rauch)
        file.close()
        print(self.rauch)
        
        file = open("/home/pi/Desktop/unfall_data_magneto.json")
        content_file = file.read()
        content = json.loads(content_file)
        self.winkel = content["MagnetoSensor"]["Winkel"]
        self.winkel = "Winkel: {}°".format(self.winkel)
        file.close()
        print(self.winkel)

        # Now repeat call
        self.after(self.TimerInterval, self.GetTemp)
        
    def licht_luft(self):
        licht_luft_pin = 27
        if GPIO.input(licht_luft_pin) == GPIO.LOW:
            GPIO.output(licht_luft_pin, GPIO.HIGH)
            print("geschalten")
        else:
            GPIO.output(licht_luft_pin, GPIO.LOW)

    def start_stop(self):
        start_stop_pin = 17
        if GPIO.input(start_stop_pin) == GPIO.LOW:
            GPIO.output(start_stop_pin, GPIO.HIGH)
            print("geschalten")
        else:
            GPIO.output(start_stop_pin, GPIO.LOW)
                
class App(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        w = 320#1280  # width for the Tk root
        h = 720#720  # height for the Tk root
        x = 0
        y = 0
        # set the title bar text
        self.title('Mama Lauda')
        # Make sure app window is big enough to show title 
        self.geometry('%dx%d+%d+%d' % (w, h, x, y))
      
        # create and pack a Mainframe window
        Mainframe(self).pack()
        
        # now start
        self.mainloop()


if __name__ == '__main__':
    licht_luft_pin = 27
    start_stop_pin = 17
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)#
    GPIO.setup(licht_luft_pin, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(start_stop_pin, GPIO.OUT, initial=GPIO.LOW)
    
    App()