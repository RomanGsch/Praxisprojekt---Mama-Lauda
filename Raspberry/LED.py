from mq import *  # import mq??
import RPi.GPIO as GPIO

pin_led = 22

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pin_led, GPIO.OUT, initial=GPIO.LOW)

mq = MQ()
sensor_rauch = mq.MQPercentage()
gas_lpg = sensor_rauch["GAS_LPG"]
co2 = sensor_rauch["CO"]
rauch = sensor_rauch["SMOKE"]

if rauch > 60:
    GPIO.output(pin_led, GPIO.HIGH)
else:
    GPIO.output(pin_led, GPIO.LOW)
