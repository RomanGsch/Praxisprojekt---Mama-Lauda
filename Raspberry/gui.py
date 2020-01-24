import tkinter as tk
import Adafruit_DHT as DHT
import time
import threading


def start():
    print('Start...')


def stop():
    print('Stop...')
    
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
    buttonStart = tk.Button(master=mainWindow, text="Starten", command=start)
    buttonStop = tk.Button(master=mainWindow, text="Stop", command=stop)
    labelTemp = tk.Label(master=mainWindow, text=temperatur)


    labelUeberschrift.pack()
    labelTemp.pack()
    buttonStart.pack()
    buttonStop.pack()
    while True:
        time.sleep(1)
        mainWindow.update()
    mainWindow.mainloop()

if __name__ == '__main__':
    main()  # TODO: muss noch checken wie man realtime updaten kann! tkinter