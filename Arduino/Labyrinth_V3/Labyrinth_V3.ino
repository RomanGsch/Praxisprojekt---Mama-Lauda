//für serielle daten
int lange = 0;
char drehung;

//Für Serial Event
String cmd_string = "";         // a String to hold incoming data
bool cmd_in_complete = false;  // whether the string is complete

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


// ------------------------------begin Program------------------------------
void serialEvent() {
  while (Serial.available()) {
    //einzelne zeichen einlesen und zusammenführen
    char char_in = (char)Serial.read();
    cmd_string += char_in;
    if (char_in == '\n') {
      cmd_in_complete = true;
    }
  }
}

void drehungL() {
  analogWrite(motorTreiberPin4, 0);
  analogWrite(motorTreiberPin3, speed2);
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, speed2);
}

void drehungR() {
  analogWrite(motorTreiberPin4, speed2);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin1, speed2);
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
  cmd_string.reserve(10); // für den inputString reservieren 10Byte
}

void loop() {
  if (cmd_in_complete) {
    if (cmd_string.charAt(4) == 88) { //88 für X
      // start -> nichts machen
      // Serial.println("Start");
    } else if (cmd_string.charAt(4) == 82 || cmd_string.charAt(4) == 76) { // 82 für R und 76 für L
      lange = (cmd_string.charAt(0)-48)*1000 + (cmd_string.charAt(1)-48)*100 + (cmd_string.charAt(2)-48)*10 + (cmd_string.charAt(0)-48);
      drehung = cmd_string.charAt(4);
      fahren(lange);
      if (drehung == 76) { // 76 für L
        Serial.println("L");
        drehungL();
      } else {
        Serial.println("R");
        drehungR();
      }
    } else if (cmd_string.charAt(4) == 84) { // 84 für T
      // nichts machen
    } else if (cmd_string.charAt(4) == 70) { // 70 für F
      // drehung beenden
      bremsen();
      // Serial.println("stop drehen");
    } else {
      // nichts machen
      // Serial.println("no data");
    }
  }
}
