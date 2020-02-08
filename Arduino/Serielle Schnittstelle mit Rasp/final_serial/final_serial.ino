int incomingByte = 0; // for incoming serial data
int langeDec[5] = {0, 0, 0, 0, 0};
int lange = 0;
int drehungDec = 0;
String drehung;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(10, OUTPUT);
}

void loop() {
  if (Serial){
  if (Serial.available() > 0) {
    for (int i=0; i < (sizeof(langeDec)/sizeof(langeDec[0])); i++){
      langeDec[i] = Serial.read();
    }
    
    lange = (langeDec[0]-48)*1000+(langeDec[1]-48)*100+(langeDec[2]-48)*10+(langeDec[3]-48);
    drehungDec = langeDec[4];
    
    if (drehungDec == 76){
      drehung = "L";
    } else if (drehungDec == 82){
      drehung = "R";
    } else {
      drehung = "no data";
    }
    if (lange == 2400) {
      digitalWrite(10, HIGH);
      delay(5000);
    } else {
      digitalWrite(10, LOW);
    }
    Serial.println("D");
    Serial.flush();
  } else {
    Serial.println("D");
  }
  }
  delay(500);
}
