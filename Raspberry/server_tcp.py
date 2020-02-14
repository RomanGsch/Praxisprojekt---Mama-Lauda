import socket
import json

# mylist = [[0000, 0000], [4000, 0000], [4000, -4000], [7000, -4000], [7000, 0000], [14000, 0000],
#          [14000, -6000], [12000, -6000], [12000, -9000], [15000, -9000], [15000, -13000]]

mylist = [[0000, 0000],  # P0
          [0000, 3500],  # P1
          [2000, 3500],  # P2
          [2000, 5500],  # P3
          [6500, 5500],  # P4
          [6500, 4500],  # P5
          [3000, 4500],  # P6
          [3000, 2000],  # P7
          [1500, 2000],  # P8
          [1500, 500],  # P9
          [5500, 500],  # P10
          [5500, 2500],  # P11
          [7000, 2500]]  # P12

js = json.dumps([(t[1], t[0]) for t in mylist], indent=4)
print(js)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("", 50000))
s.listen(1)
try:
    while True:
        komm, addr = s.accept()
        while True:
            data = komm.recv(1024)
            if not data:
                komm.close()
                break
            print("[{}] {}".format(addr[0], data.decode()))
            # nachricht = input("Antwort: ")
            komm.send(js.encode())
finally:
    s.close()
