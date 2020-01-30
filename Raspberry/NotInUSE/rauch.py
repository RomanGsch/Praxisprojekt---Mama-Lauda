from mq import *  # import mq??
import RPi.GPIO as GPIO

pin_led = 22
pin_rauch = 5

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(pin_led, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(pin_rauch, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

mq = MQ()
sensor_rauch = mq.MQPercentage()
gas_lpg = sensor_rauch["GAS_LPG"]
co2 = sensor_rauch["CO"]
rauch = sensor_rauch["SMOKE"]

if rauch > 60:
    GPIO.output(pin_led, GPIO.HIGH)
else:
    GPIO.output(pin_led, GPIO.LOW)
