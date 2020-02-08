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


void richtung(char drehung){
  if (drehung == "R"){
    Serial.println("Rechts Ausweichen");
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed2);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed2);
    delay(800);
    bremsen();
  } 
  else if (drehung == "L"){
    Serial.println("Links Ausweichen - Drehung");
    analogWrite(motorTreiberPin4, speed2);
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin1, speed2);
    delay(800);
    bremsen();
  }
}

void fahren(int strecke, char drehung){
  analogWrite(motorTreiberPin1, speed1);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, speed1);
  analogWrite(motorTreiberPin4, 0);
  Serial.println("Fahren");
  delay(strecke);
  bremsen();
  richtung(drehung);
}

void bremsen(){
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin4, 0);
  delay(1000);
  Serial.println("Bremsen");
}

void setup(){
    Serial.begin(9600);
    pinMode(motorTreiberPin1, OUTPUT);
    pinMode(motorTreiberPin3, OUTPUT);
}

int geradefahren[]={1000,2000,3000};
int richtungen[]={1,2,1};
int folgenAnzahl = 3;
char loesung;

void loop(){
  for (int labyrinth=0; labyrinth<folgenAnzahl; labyrinth=labyrinth+1){
    loesung(3, richtung[labyrinth]);
    delay(geradefahren[labyrinth]);
    //Serial.println(geradefahren);

    //Serial.println(richtung);
  }
}
//void loop(){
  //if (digitalRead(start)){
   // fahren(1000, "L");}
//}
