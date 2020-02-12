import socket
import json

mylist = [[0000, 0000], [4000, 0000], [4000, -4000], [7000, -4000], [7000, 0000], [14000, 0000],
          [14000, -6000], [12000, -6000], [12000, -9000], [15000, -9000], [15000, -13000]]

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
