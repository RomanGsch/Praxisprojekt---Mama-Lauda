//US1-Mitte
const int triggerPinM = 4;
const int echoPinM = 5;

//US2-Links
const int triggerPinL = 2;
const int echoPinL = 3;

//US3-Rechts
const int triggerPinR = 10;
const int echoPinR = 11;

//Motor 1
const int motorPin1 = 7;
const int motorPin2  = 6;

//Motor 2
const int motorPin3  = 9;
const int motorPin4  = 8;

long dauer, abst, abstLR, Abstandlinks, Abstandrechts;

int speed1 = 180;
int speed2 = 180;

const int RechtsForward = 200; //max Speed soll lt. Datenblatt bei 180 sein
const int RechtsBackward = 180;
const int LinksForward = 200;
const int LinksBackward = 180;

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

void Abstandsvergleich(){
  if (Abstandlinks > Abstandrechts){
    analogWrite(motorPin4, LinksBackward);
    analogWrite(motorPin1, RechtsForward);
    Serial.println("Rechts Ausweichen");
    delay(1000);
    analogWrite(motorPin3, LinksForward);
    analogWrite(motorPin2, RechtsForward);
    delay(1000);
  }
  else if (Abstandrechts > Abstandlinks){
    analogWrite(motorPin3, LinksForward);
    analogWrite(motorPin2, RechtsBackward);
    Serial.println("Links Ausweichen");
    delay(1000);
    analogWrite(motorPin3, LinksForward);
    analogWrite(motorPin1, RechtsForward);
    delay(1000);
  }
  else{
    analogWrite(motorPin3, LinksForward);
    analogWrite(motorPin2, RechtsBackward);
    delay(1000);
    Serial.println("180° Drehung");
  }
}


void fahren(){
  //Motor Control A in both directions
  analogWrite(motorPin1, speed1);
  //Motor Control B in both directions
  analogWrite(motorPin3, speed2);
  Serial.println("Fahren");
}

void bremsen(){
  //Motor Control A in both directions
  analogWrite(motorPin1, 0);
  //Motor Control B in both directions
  analogWrite(motorPin3, 0);
  Serial.println("Bremsen");
}

void rechts(){
  //Motor Control A in both directions
  analogWrite(motorPin1, 0);
  //Motor Control B in both directions
  analogWrite(motorPin3, speed1);
  delay(1000);
  Serial.println("Rechts");
}


void setup(){
    Serial.begin(9600);
    //Set pins as outputs
    pinMode(motorPin1, OUTPUT);
    //pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    //pinMode(motorPin4, OUTPUT);
    pinMode(triggerPinM, OUTPUT);
    pinMode(echoPinM, INPUT);
    pinMode(triggerPinL, OUTPUT);
    pinMode(echoPinL, INPUT);
    pinMode(triggerPinR, OUTPUT);
    pinMode(echoPinR, INPUT);
}

void loop(){
  abst = messen(triggerPinM, echoPinM, "Mitte");
  
  if(abst<=30){
    bremsen();
    delay(1000);
    Abstandlinks = pingLR(triggerPinL, echoPinL, "Links");
    delay(10);
    Abstandrechts = pingLR(triggerPinR, echoPinR, "Rechts");
    delay(10);
    Abstandsvergleich();
  }
  
  else{
    fahren();   
  }
}
