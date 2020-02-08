//für serielle daten
int langeDec[5] = {0, 0, 0, 0, 0};
int lange = 0;
int drehungDec = 0;
String drehung;
bool dreh_ninty = 1;

//Motor 1
const int motorTreiberPin1 = 5; //motorTreiberPin1 -> ArduinoOut5
const int motorTreiberPin2  = 6; //motorTreiberPin2 -> ArduinoOut6

//Motor 2
const int motorTreiberPin3  = 11; //motorTreiberPin3 -> ArduinoOut10
const int motorTreiberPin4  = 10; //motorTreiberPin4 -> ArduinoOut11

//Startsignal RaspPI
char start = A0;

//Geschwindigkeitseinstellung
int speed1 = 200; //Fahren
int speed2 = 255; //Drehen


void richtung(String drehung){
  if (drehung == "L"){
    Serial.println("L");
    delay(100);
    if (Serial.available()>0){
      dreh_ninty = Serial.read();
      while (dreh_ninty == 1) {
        analogWrite(motorTreiberPin4, 0);
        analogWrite(motorTreiberPin3, speed2);
        analogWrite(motorTreiberPin1, 0);
        analogWrite(motorTreiberPin2, speed2);
        if (Serial.available()>0){
          dreh_ninty = Serial.read();
        }
      }
    }
    //Serial.println("D");
    //Serial.flush();
    // delay(2500); // feinjustiert unterschiede mit den motoren
    bremsen();
  }
  else if (drehung == "R"){
    Serial.println("R");
    delay(100);
    if (Serial.available() > 0) {
      dreh_ninty = Serial.read();
      while (dreh_ninty == 0) {
        analogWrite(motorTreiberPin4, speed2);
        analogWrite(motorTreiberPin3, 0);
        analogWrite(motorTreiberPin2, 0);
        analogWrite(motorTreiberPin1, speed2);
        delay(100);
        dreh_ninty = Serial.read();
      }
    }
    // Serial.flush();
    // delay(2300); // feinjustiert unterschiede mit den motoren
    bremsen();
    //Serial.println("D"); // für done
  }
}

void fahren(int strecke, String drehung){
  analogWrite(motorTreiberPin1, speed1);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, speed1);
  analogWrite(motorTreiberPin4, 0);
  // Serial.println("Fahren");
  delay(strecke);
  bremsen();
  richtung(drehung);
  // Serial.println("D");
}

void bremsen(){
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin4, 0);
  delay(500);
  Serial.println("Bremsen");
}

void setup(){
    Serial.begin(9600);
    pinMode(motorTreiberPin1, OUTPUT);
    pinMode(motorTreiberPin3, OUTPUT);
}

void loop() {
  if (Serial) {
    if (Serial.available() > 0) {
      for (int i=0; i < (sizeof(langeDec)/sizeof(langeDec[0])); i++){
        langeDec[i] = Serial.read();
      }
      
      lange = (langeDec[0]-48)*1000+(langeDec[1]-48)*100+(langeDec[2]-48)*10+(langeDec[3]-48);
      drehungDec = langeDec[4];
      
      if (drehungDec == 76) {
        drehung = "L";
      } else if (drehungDec == 82) {
        drehung = "R";
      } else {
        drehung = "no data";
      }
      fahren(lange, drehung);
    }
      Serial.println("D");
      Serial.flush();
    }
  delay(500);
 }
