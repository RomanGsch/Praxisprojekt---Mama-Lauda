//Libraries für den Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS A1

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//Lanze
int tempLanze;

//start
int startPinRP = A0;

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
const int motorTreiberPin3  = 11;//motorTreiberPin3 -> ArduinoOut10
const int motorTreiberPin4  = 10; //motorTreiberPin4 -> ArduinoOut11

long dauer, abst, abstLR, abstL, abstR, abstlinks, abstrechts;

int speed1 = 200;
int speed2 = 255;

//int speed2 = 200; //eventuell erst benötigt wenn ausgleich der beiden seiten sein muss (gerade aus fahren)

//zur Messung von mit US
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
    Serial.println("Objekt umfahren");
    analogWrite(motorTreiberPin1, speed1);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin4, 0);
    delay(1000);
    Serial.println("Drehung - Rechts Ausweichen - Ausgleich");
    analogWrite(motorTreiberPin4, speed2);
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin1, speed2);
    delay(1000);
  }
  else if (abstR > abstL){
    Serial.println("Links Ausweichen - Drehung");
    analogWrite(motorTreiberPin4, speed2);
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin1, speed2);
    delay(1000);
    Serial.println("Objekt umfahren");
    analogWrite(motorTreiberPin1, speed1);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin4, 0);
    delay(1000);
    Serial.println("Drehung - Links Ausweichen - Ausgleich");
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed2);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed2);
    delay(1000);
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
  //Motor Control A in both directions
  analogWrite(motorTreiberPin1, speed1);
  analogWrite(motorTreiberPin2, 0);
  //Motor Control B in both directions
  analogWrite(motorTreiberPin3, speed1);
  analogWrite(motorTreiberPin4, 0);
  Serial.println("Fahren");
}

void bremsen(){
  //Motor Control A in both directions
  analogWrite(motorTreiberPin1, 0);
  //Motor Control B in both directions
  analogWrite(motorTreiberPin3, 0);
  delay(1000);
  Serial.println("Bremsen");
}

void templanze(){
    Serial.print("Temperatur an Lanze: ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" °C");
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin4, 0);
    delay(200);
    Serial.println("Temperatur-Lanze STOP");
    analogWrite(motorTreiberPin2, speed1); //Motoren Rückwärts
    analogWrite(motorTreiberPin4, speed1);
    delay(500);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin4, 0);
    delay(2000000); //Motorstop an der Unfallstelle
  }

void lowspeed(){
  //Motor Control A in both directions
  analogWrite(motorTreiberPin1, speed2);
  analogWrite(motorTreiberPin2, 0);
  //Motor Control B in both directions
  analogWrite(motorTreiberPin3, speed2);
  analogWrite(motorTreiberPin4, 0);
  Serial.println("Fahren Low Speed");
}

void setup(){
    Serial.begin(9600);
    //Set pins as outputs
    pinMode(motorTreiberPin1, OUTPUT);
    //pinMode(motorTreiberPin2, OUTPUT);
    pinMode(motorTreiberPin3, OUTPUT);
    //pinMode(motorTreiberPin4, OUTPUT);
    pinMode(triggerPinM, OUTPUT);
    pinMode(echoPinM, INPUT);
    pinMode(triggerPinL, OUTPUT);
    pinMode(echoPinL, INPUT);
    pinMode(triggerPinR, OUTPUT);
    pinMode(echoPinR, INPUT);
    pinMode(triggerPinH, OUTPUT);
    pinMode(echoPinH, INPUT);
    pinMode(startPin, INPUT);
    sensors.begin();

}

void loop(){
  while(digitalRead(startPinRP) == HIGH){
    abst = 0;
    abst = messen(triggerPinM, echoPinM, "Abstand von US-Mitte");
    fahren();
    sensors.requestTemperatures();
    tempLanze = (sensors.getTempCByIndex(0));
    if(abst<=50){
      bremsen();
      abstL = pingLR(triggerPinL, echoPinL, "Echo-Links");
      delay(200);
      abstR = pingLR(triggerPinR, echoPinR, "Echo-Rechts");
      delay(200);
      abstVgl(abstL, abstR);
      Serial.print("Temperatur an Lanze: ");
      Serial.print(sensors.getTempCByIndex(0));
      Serial.println(" °C");
    }
    else{
      fahren();
    }
  }
}
