import py_qmc5883l
import time
# TODO: kalibrierung einbauen
sensor = py_qmc5883l.QMC5883L()
startwinkel = int(sensor.get_bearing())
minusRange = startwinkel - 45
plusRange = startwinkel +45

entfernung = 0

print("startwinkel = {}".format(startwinkel))
print("minusRange = {}".format(minusRange))
print("plusRange = {}".format(plusRange))

while True:
    messwinkel = int(sensor.get_bearing())
    # print("messwinkel = {}".format(messwinkel))
    # if (messwinkel > minusRange) and (messwinkel < plusRange):
    if messwinkel in range(minusRange, plusRange):
        entfernung += 1
    print("Messwert: {}/Winkel: {}".format(entfernung, messwinkel))
    time.sleep(1)
