import RPi.GPIO as GPIO
import threading
import time
import random
 
R = 24
G = 23
B = 18
 
PINS = [R,G,B]

 
def initialize_gpio():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(PINS, GPIO.OUT, initial=GPIO.LOW)
 
 
def color_test(channel, frequency, speed, step, duty):
    p = GPIO.PWM(channel, frequency)
    p.start(0)

    while True:
        p.ChangeDutyCycle(duty)
        time.sleep(speed)
 
def color_test_thread():
    threads = []
    threads.append(threading.Thread(target=color_test, args=(R, 1000, 1, 5, 100)))
    threads.append(threading.Thread(target=color_test, args=(G, 1000, 1, 5, 100)))
    threads.append(threading.Thread(target=color_test, args=(B, 1000, 1, 5, 100)))
    for t in threads:
        t.daemon = True
        t.start()
    for t in threads:
        t.join()
 
 
def main():
    try:
        initialize_gpio()
        print("STRG + C FÜR ABBRUCH!")
        color_test_thread()
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()
 
 
if __name__ == '__main__':
    main()
