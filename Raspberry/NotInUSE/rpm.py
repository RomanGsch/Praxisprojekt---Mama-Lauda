import RPi.GPIO as GPIO
import time

counter = 0
pin_rpm = 6  # lt. schaltplan
led_pin = 4

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(pin_rpm, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(led_pin, GPIO.OUT, initial=GPIO.LOW)
GPIO.output(led_pin, GPIO.HIGH)

last_state = False

while True:
    current_state = GPIO.input(pin_rpm)
    print(counter)
    if current_state != last_state:
        counter += 1
        last_state = current_state
    