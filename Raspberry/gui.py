import tkinter as tk
import Adafruit_DHT as DHT
import time
import threading
import RPi.GPIO as GPIO

def licht_luft():
    licht_luft_pin = 4
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

def temp():
    sensor_temp = DHT.DHT11
    pin_temp = 27
    count = 0
    feuchtigkeit, temperatur = DHT.read(sensor_temp, pin_temp)
    print('measure', temperatur)
    return temperatur


def main():
    #threads = []
    #threads.append(threading.Thread(target=temp))
    mainWindow = tk.Tk(className="Mamma Lauda")
    w = 320#1280  # width for the Tk root
    h = 720#720  # height for the Tk root
    x = 0
    y = 0
    temperatur = temp()
    mainWindow.geometry('%dx%d+%d+%d' % (w, h, x, y))
    labelUeberschrift = tk.Label(master=mainWindow, text="Team --- MAMMA LAUDA")
    buttonStart = tk.Button(master=mainWindow, text="Starten", command=start_stop)
    buttonStop = tk.Button(master=mainWindow, text="Stop", command=start_stop)
    buttonLicht = tk.Button(master=mainWindow, text="Licht", command=licht_luft)
    labelTemp = tk.Label(master=mainWindow, text=temperatur)


    labelUeberschrift.pack()
    labelTemp.pack()
    buttonStart.pack()
    buttonStop.pack()
    buttonLicht.pack()
    while True:
        time.sleep(1)
        mainWindow.update()
    mainWindow.mainloop()

if __name__ == '__main__':
    
    licht_luft_pin = 4
    start_stop_pin = 17
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)#
    GPIO.setup(licht_luft_pin, GPIO.OUT, initial=GPIO.LOW)
    GPIO.setup(start_stop_pin, GPIO.OUT, initial=GPIO.LOW)
    
    main()  # TODO: muss noch checken wie man realtime updaten kann! tkinter