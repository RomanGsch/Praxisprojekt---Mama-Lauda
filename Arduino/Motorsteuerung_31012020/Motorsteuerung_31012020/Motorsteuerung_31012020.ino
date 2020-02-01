//Libraries für den Sensor
#include <OneWire.h>
#define ONE_WIRE_BUS A1

//US1-Mitte
const int triggerPinM = 3;
const int echoPinM = 8;

//US2-Links
const int triggerPinL = 4;
const int echoPinL = 9;

//US3-Rechts
const int triggerPinR = 2;
const int echoPinR = 7;

//US4-Hinten
const int triggerPinH = 12;
const int echoPinH = 13;

//Motor 1
const int motorTreiberPin1 = 5; //motorTreiberPin1 -> ArduinoOut5
const int motorTreiberPin2  = 6; //motorTreiberPin2 -> ArduinoOut6

//Motor 2
const int motorTreiberPin3  = 11; //motorTreiberPin3 -> ArduinoOut10
const int motorTreiberPin4  = 10; //motorTreiberPin4 -> ArduinoOut11

long dauer, abst, abstLR, abstL, abstR, abstlinks, abstrechts;

//Startsignal RaspPI
char start = A0;

//Geschwindigkeitseinstellung
int speed1 = 200; //Fahren
int speed2 = 255; //Drehen

//Messung über mittleren US
long messen(int triggerPin, int echoPin, String Richtung){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(3);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  dauer = pulseIn(echoPin, HIGH);
  abst = (dauer/2)/25;
  Serial.println(Richtung);
  Serial.println(abst);
  return abst;
}
//Abstandsvergleich Links/Rechts
long pingLR(int triggerPin, int echoPin, String Richtung){
  abstLR = messen(triggerPin, echoPin, Richtung);
  return abstLR;
}

void abstVgl(int abstL, int abstR){
  if (abstL > abstR){
    Serial.println("Rechts Ausweichen");
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed2);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed2);
    delay(1000);
    bremsen();
    Serial.println("Objekt umfahren");
    analogWrite(motorTreiberPin1, speed1);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin4, 0);
    delay(2000);
    bremsen();
    Serial.println("Drehung - Rechts Ausweichen - Ausgleich");
    analogWrite(motorTreiberPin4, speed2);
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin1, speed2);
    delay(1000);
    bremsen();
  }
  else if (abstR > abstL){
    Serial.println("Links Ausweichen - Drehung");
    analogWrite(motorTreiberPin4, speed2);
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin1, speed2);
    delay(1000);
    bremsen();
    Serial.println("Objekt umfahren");
    analogWrite(motorTreiberPin1, speed1);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin4, 0);
    delay(2000);
    bremsen();
    Serial.println("Drehung - Links Ausweichen - Ausgleich");
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed2);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed2);
    delay(1000);
    bremsen();
  }
  else{
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed2);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed2);
    delay(2000);
    Serial.println("180° Drehung");
  }
}

void fahren(){
  analogWrite(motorTreiberPin1, speed1);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, speed1);
  analogWrite(motorTreiberPin4, 0);
  Serial.println("Fahren");
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
    pinMode(triggerPinM, OUTPUT);
    pinMode(echoPinM, INPUT);
    pinMode(triggerPinL, OUTPUT);
    pinMode(echoPinL, INPUT);
    pinMode(triggerPinR, OUTPUT);
    pinMode(echoPinR, INPUT);
    pinMode(triggerPinH, OUTPUT);
    pinMode(echoPinH, INPUT);
    pinMode(start, INPUT);
}

void loop(){
  abst = messen(triggerPinM, echoPinM, "Mitte messen");
  if(digitalRead(start == 1)){
    fahren();
    }
  else if(abst<=30){
    bremsen();
    abstL = pingLR(triggerPinL, echoPinL, "Links messen - Ping");
    delay(200);
    abstR = pingLR(triggerPinR, echoPinR, "Rechts messen - Ping");
    delay(200);
    abstVgl(abstL, abstR);
  }
  else{
    bremsen();
  }
}
