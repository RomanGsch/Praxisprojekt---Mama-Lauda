import RPi.GPIO as GPIO

counter = 0
pin_rpm_left = 6  # lt. schaltplan
pin_rpm_right = 13  # lt. schaltplan

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pin_rpm_left, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(pin_rpm_right, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

rpm_left = GPIO.input(pin_rpm_left)
rpm_right = GPIO.input(pin_rpm_right)

if (rpm_left and rpm_right) is True:
    counter += 1
    print(counter)