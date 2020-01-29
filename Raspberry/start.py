import RPi.GPIO as GPIO
import time

start_pin = 4  # 17

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)#
GPIO.setup(start_pin, GPIO.OUT, initial=GPIO.LOW)
print("low")
time.sleep(5)

try:
    while True:
        GPIO.output(start_pin, GPIO.HIGH)
        print("high")
        time.sleep(5)
        GPIO.output(start_pin, GPIO.LOW)
        print("low")
        time.sleep(5)
finally:
    GPIO.cleanup()
    
