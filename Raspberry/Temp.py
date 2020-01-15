import Adafruit_DHT as DHT

sensor_temp = DHT.DHT11
pin_temp = 27
humidity, temperature = DHT.read_retry(sensor_temp, pin_temp)