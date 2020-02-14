                """
                elif str(self.cmd) == "R\r\n":
                    try:
                        file = open("/home/pi/Desktop/unfall_data_magneto.json")
                        content_file = file.read()
                        content = json.loads(content_file)
                        self.degrees1 = content["MagnetoSensor"]["Winkel"]
                        file.close()
                    except Exception as e:
                        print("Deg1_Error: {}".format(e))
                    while True:
                        try:
                            file = open("/home/pi/Desktop/unfall_data_magneto.json")
                            content_file = file.read()
                            content = json.loads(content_file)
                            self.degrees2 = content["MagnetoSensor"]["Winkel"]
                            file.close()
                        except Exception as e:
                            print("Deg2_Error: {}".format(e))
                        deg_calc = abs(self.degrees1 - self.degrees2)
                        if abs(deg_calc) < 90:
                            ser.write("1".encode("ascii"))
                            print("turning: {}".format(deg_calc))
                        else:
                            ser.write("0".encode("ascii"))
                            print("stop turning")
                        sleep(0.25)
                """