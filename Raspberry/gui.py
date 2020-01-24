import tkinter as tk

def main():
    mainWindow = tk.Tk(className="Mamma Lauda")
    w = 320#1280  # width for the Tk root
    h = 720#720  # height for the Tk root
    x = 0
    y = 0
    mainWindow.geometry('%dx%d+%d+%d' % (w, h, x, y))

    #mainWindow.geometry("500x500")
    #mainWindow.resizable(width=None, height=None)
    #mainWindow.
    mainWindow.mainloop()

if __name__ == '__main__':
    main()