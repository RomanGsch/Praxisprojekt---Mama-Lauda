int x = 0;
int y = 0;

void setup(){
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop(){
    if (Serial.available() > 0) {
        Serial.write("loop_begin");
        if (Serial.read() == "data_ready") {
            Serial.write("ready_strecke");
            strecke = Serial.read();
        if (Serial.read() ==
        }
            strecke = Serial.read();
            strecke = toint(strecke);
            Serial.print("ready_drehung");
            drehung = Serial.read();
            Serial.println(strecke);
            Serial.println(drehung);
            // fahren(strecke, drehung)
          }
      }
    }
  delay(1000);
}
