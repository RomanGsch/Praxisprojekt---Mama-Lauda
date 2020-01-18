import Adafruit_DHT as DHT
import time

sensor_temp = DHT.DHT11
pin_temp = 27
count = 0
feuchtigkeit, temperatur = DHT.read(sensor_temp, pin_temp)
while True:
    print("{}. Temperatur: {}, Luftfeuchtigkeit: {}".format(count, temperatur, feuchtigkeit))
    count += 1
    time.sleep(1)