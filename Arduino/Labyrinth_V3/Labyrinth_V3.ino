//für serielle daten
int lange = 0;
char drehung;
int drehungDec = 0;
int langeDec[5] = {0, 0, 0, 0, 0};

//Für Serial Event
String cmd_string = "";         // a String to hold incoming data
bool cmd_in_complete = true;  // whether the string is complete

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

void drehungL() {
  analogWrite(motorTreiberPin4, 0);
  analogWrite(motorTreiberPin3, speed2);
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, speed2);
  delay(2000);
}

void drehungR() {
  analogWrite(motorTreiberPin4, speed2);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin1, speed2);
  delay(2000);
}

void fahren(int strecke){
  analogWrite(motorTreiberPin1, speed1);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, speed1);
  analogWrite(motorTreiberPin4, 0);
  delay(strecke);
  bremsen();
}

void bremsen(){
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin4, 0);
}

void setup(){
  Serial.begin(9600);
  pinMode(motorTreiberPin1, OUTPUT);
  pinMode(motorTreiberPin3, OUTPUT);
  //cmd_string.reserve(10); // für den inputString reservieren 10Byte
}

void loop() {
  if (Serial){
    if (Serial.available() > 0) {
      for (int i=0; i < (sizeof(langeDec)/sizeof(langeDec[0])); i++){
        langeDec[i] = Serial.read();
      }
      lange = (langeDec[0]-48)*1000+(langeDec[1]-48)*100+(langeDec[2]-48)*10+(langeDec[3]-48);
      drehungDec = langeDec[4];

      fahren(lange);
      bremsen();
      //Serial.print("vor Delay");
      delay(1000);
      Serial.flush();
      if (drehung == 76) { // 76 für L
        Serial.println("L");
        drehungL();
      } else if (drehung == 82) {
        Serial.println("R");
        drehungR();
      } else if (drehung == 84) { // 84 für T
      // nichts machen
      } else if (drehung == 70) { // 70 für F
      // drehung beenden
      bremsen();
      // Serial.println("stop drehen");
      }
    }
  }
  Serial.println("L");
  Serial.flush();
  bremsen();
  delay(500);
}
