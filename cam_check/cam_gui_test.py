import picamera
from time import sleep


with picamera.PiCamera() as camera:
        camera.preview_fullscreen=False
        camera.preview_window=(620, 320, 640, 480)

        camera.resolution=(640,480)
        camera.start_preview()
        camera.sharpness = 10
        camera.contrast = 30
        camera.vflip=False
        camera.hflip=False
        camera.exposure_mode = 'auto'

        sleep(100)
        #camera.stop_preview()
        #camera.close()