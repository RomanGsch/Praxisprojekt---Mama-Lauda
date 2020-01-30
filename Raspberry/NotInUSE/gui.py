from tkinter import *
import Adafruit_DHT as DHT
import time
import threading
import RPi.GPIO as GPIO
from w1thermsensor import W1ThermSensor

def licht_luft():
    licht_luft_pin = 27
    if GPIO.input(licht_luft_pin) == GPIO.LOW:
        GPIO.output(licht_luft_pin, GPIO.HIGH)
        print("geschalten")
    else:
        GPIO.output(licht_luft_pin, GPIO.LOW)

def start_stop():
    start_stop_pin = 17
    if GPIO.input(start_stop_pin) == GPIO.LOW:
        GPIO.output(start_stop_pin, GPIO.HIGH)
        print("geschalten")
    else:
        GPIO.output(start_stop_pin, GPIO.LOW)

class Temperatur(threading.Thread):
    def __init__(self, mainWindow):
        threading.Thread.__init__(self)
        self.deamon = True
        self.mainWindow = mainWindow
        self.temperatur = 0
        
    def run(self):
        global temperatur
        temp_sens = W1ThermSensor()
        self.temperatur = temp_sens.get_temperature()
        print(self.temperatur)
        #time.sleep(0.5)
        self.mainWindow.after(500, self.run)    

def main():
    global temperatur
    global mainWindow
    #temperatur = 0
    threads = []
    
    #gui_thread = threading.Thread(target)

    
    
    mainWindow = Tk(className="Mamma Lauda")
    w = 320#1280  # width for the Tk root
    h = 720#720  # height for the Tk root
    x = 0
    y = 0
    
    
    temp_thread = Temperatur(mainWindow)
    temp_thread.start()
    
    
    threads.append(temp_thread)

    time.sleep(1)
    mainWindow.geometry('%dx%d+%d+%d' % (w, h, x, y))
    labelUeberschrift = Label(master=mainWindow, text="Team --- MAMMA LAUDA")
    buttonStart = Button(master=mainWindow, text="Starten", command=start_stop)
    buttonStop = Button(master=mainWindow, text="Stop", command=start_stop)
    buttonLicht = Button(master=mainWindow, text="Licht", command=licht_luft)
    labelTemp = Label(master=mainWindow, text=temp_thread.temperatur)


    labelUeberschrift.pack()
    labelTemp.pack()
    buttonStart.pack()
    buttonStop.pack()
    buttonLicht.pack()
    
    while True:
        mainWindow.update()
    
    mainWindow.mainloop()

if __name__ == '__main__':
    
    licht_luft_pin = 27
    start_stop_pin = 17
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)#
    GPIO.setup(licht_luft_pin, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(start_stop_pin, GPIO.OUT, initial=GPIO.LOW)
    
    main()  # TODO: muss noch checken wie man realtime updaten kann! tkinter