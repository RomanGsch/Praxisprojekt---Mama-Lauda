import serial

ser = serial.Serial("/dev/cu.usbmodem1411", 9600, timeout=2)  # change ACM number as found from ls /dev/tty/ACM*
ser.baudrate = 9600

while True:
    text = input("Eingabe: ")  # "R"
    text_ascii = text.encode("ascii")  # + bytes([13, 10])
    write_ser = ser.write(text_ascii)
    text = ser.readline().decode("ascii")
    print("Return: " + text)
    while text is not "":
        text = ser.readline().decode("ascii")
        print("Return: " + text)