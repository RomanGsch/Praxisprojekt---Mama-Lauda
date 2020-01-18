import picamera
from time import sleep

with picamera.PiCamera() as cam:
    cam.resolution = (3280, 2464)
    cam.start_preview()
    #cam.start_recording('filmchen.h264')
    #cam.wait_recording(30)
    #cam.stop_recording()
    sleep(5)
    cam.stop_preview()
    cam.close()

""" # unsere lösung
import picamera as pic
import time

cam = pic.PiCamera()
cam.resolution = (3280, 2464)
cam.start_preview()
time.sleep(5)
cam.stop_preview()
cam.capture('bild.jpg')
cam.close()
"""