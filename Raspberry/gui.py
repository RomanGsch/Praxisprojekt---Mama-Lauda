import tkinter as tk


def start():
    print('Start...')


def stop():
    print('Stop...')


def main():
    mainWindow = tk.Tk(className="Mamma Lauda")
    w = 320#1280  # width for the Tk root
    h = 720#720  # height for the Tk root
    x = 0
    y = 0
    Temp = 100
    mainWindow.geometry('%dx%d+%d+%d' % (w, h, x, y))
    labelUeberschrift = tk.Label(master=mainWindow, text="Team --- MAMMA LAUDA")
    buttonStart = tk.Button(master=mainWindow, text="Starten", command=start)
    buttonStop = tk.Button(master=mainWindow, text="Stop", command=stop)
    labelTemp = tk.Label(master=mainWindow, text=Temp)


    labelUeberschrift.pack()
    labelTemp.pack()
    buttonStart.pack()
    buttonStop.pack()

    mainWindow.mainloop()

if __name__ == '__main__':
    main()