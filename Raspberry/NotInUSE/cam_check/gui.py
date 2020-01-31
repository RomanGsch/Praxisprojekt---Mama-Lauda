from tkinter import *
import RPi.GPIO as GPIO
from PIL import Image, ImageTk
from subprocess import Popen, PIPE

class Tela(object):
        def __init__(self,master, **kwargs):
                self.master=master
                pad=3
                self.geom='200x200+0+0'
                master.geometry("{0}x{1}+0+0".format(master.winfo_screenwidth()-pad,
master.winfo_screenheight()-pad))




        def toggle_geom(self, event):
                geom=self.master.winfo_geometry()
                (geom,self._geom)
                self.master.geometry(self._geom)
                self._geom=geom

win = Tk()
win.title("test")
app=Tela(win)
frame = Frame(win)


frame.pack()

proc=Popen(["python","cam_gui_test.py"],stdout=PIPE)
output=proc.communicate()[0]
print(output)


win.mainloop()