import time
from w1thermsensor import W1ThermSensor

temp_sens = W1ThermSensor()

while True:
    Temperatur = temp_sens.get_temperature()
    print(Temperatur)
    time.sleep(1)