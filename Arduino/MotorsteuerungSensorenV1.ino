#include <HC_SR04.h>
HC_SR04 sensor(triggerPin, echoPin);

//Ultraschallsensor Mitte
const int triggerPin1 = 4;
const int echoPin1 = 5;
long dauer, abst;

//Ultraschallsensor Rechts
const int triggerPin2 = 10;
const int echoPin2 = 11;
int Abstandrechts;
 
//Ultraschallsensor Links
const int triggerPin3 = 0;
const int echoPin3 = 1;
int Abstandlinks;

//Motoren Rechts
const int motorPin1 = 9; //Vorwärts
const int motorPin2 = 8; //Rückwärts

//Motoren Links
const int motorPin3 = 7; //Vorwärts
const int motorPin4 = 6; //Rückwärts

const int RechtsForward = 120; //max Speed soll lt. Datenblatt bei 180 sein
const int RechtsBackward = 60;
const int LinksForward = 120;
const int LinksBackward = 60;


//Abstandsmessung der Ultraschallsensoren
long messen(){
  digitalWrite(triggerPin1, LOW);
  delayMicroseconds(3);
  digitalWrite(triggerPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin1, LOW);
  
  dauer = pulseIn(echoPin1, HIGH);
  abst = (dauer/2)/25;
  Serial.println(abst);
  return abst;
}

void fahren(){
  //Motor Control A in both directions
  analogWrite(motorPin1, RechtsForward);
  //Motor Control B in both directions
  analogWrite(motorPin3, LinksForward);
  Serial.println("Fahren");
}

void bremsen(){
  //Motor Control A in both directions
  analogWrite(motorPin1, 0);
  //Motor Control B in both directions
  analogWrite(motorPin3, 0);
  Serial.println("Bremsen");
}

//void rechts(){
//  //Motor Control A in both directions
//  analogWrite(motorPin1, 0);
//  //Motor Control B in both directions
//  analogWrite(motorPin3, speed1);
//  delay(1000);
//  Serial.println("Rechts");
//}

//void links(){
//  //Motor Control A in both directions
//  analogWrite(motorPin3, 0);
//  //Motor Control B in both directions
//  analogWrite(motorPin1, speed1);
//  delay(1000);
//  Serial.println("Links");
//}


void setup(){
    Serial.begin(9600);
    //Set pins as outputs
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
    pinMode(triggerPin1, OUTPUT);
    pinMode(triggerPin2, OUTPUT);
    pinMode(triggerPin3, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(echoPin2, INPUT);
    pinMode(echoPin3, INPUT);
}

long ping(){ //Speichern der Abstandsdaten vom rechten und linken Ultraschallsensor
   sensor.start();
   while(!sensor.isFinished()) continue;
   return sensor.getRange();
  }
  
void Abstandsvergleich(){
  if (Abstandlinks > Abstandrechts){
    motorpin4.write(LinksBackward);
    motorpin1.write(RechtsForward);
    Serial.println("Rechts Ausweichen");
    delay(400);
    motorpin3.write(LinksForward);
    motorpin1.write(RechtsForward);
    delay(600);
  }
  else if (Abstandrechts > Abstandlinks({
    motorpin3.write(LinksForward);
    motorpin2.write(RechtsBackward);
    Serial.println("Links Ausweichen");
    delay(400);
    motorpin3.write(LinksForward);
    motorpin1.write(RechtsForward);
    delay(600);
  }
  else{
    motorpin3.write(LinksForward);
    motorpin2.write(Rechtsbackward);
    delay(400);
    Serial.println("180° Drehung");
  }
}

void loop(){
  //abst = messen();
  digitalWrite(triggerPin1, LOW);
  delayMicroseconds(3);
  digitalWrite(triggerPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin1, LOW);
  
  dauer = pulseIn(echoPin1, HIGH);
  abst = (dauer/2)/25;
  Serial.println(abst);

  if(abst<=30){
    bremsen();
    delay(1000);
    Abstandlinks = ping();
    delay(200);
    Abstandrechts = ping();
    delay(200);
    Abstandsvergleich();
  }
  
  else{
    fahren();   
  }
}
