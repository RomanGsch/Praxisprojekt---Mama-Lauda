//US1-Mitte
const int triggerPinM = 3;
const int echoPinM = 8;

//US2-Links
const int triggerPinL = 2;
const int echoPinL = 7;

//US3-Rechts
const int triggerPinR = 4;
const int echoPinR = 9;

//Motor 1
const int motorTreiberPin1 = 5; //motorTreiberPin1 -> ArduinoOut5
const int motorTreiberPin2  = 6; //motorTreiberPin2 -> ArduinoOut6

//Motor 2
const int motorTreiberPin3  = 10;//motorTreiberPin3 -> ArduinoOut10
const int motorTreiberPin4  = 11; //motorTreiberPin4 -> ArduinoOut11

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
    analogWrite(motorTreiberPin4, LinksBackward);
    analogWrite(motorTreiberPin1, RechtsForward);
    Serial.println("Rechts Ausweichen");
    delay(1000);
    analogWrite(motorTreiberPin3, LinksForward);
    analogWrite(motorTreiberPin2, RechtsForward);
    delay(1000);
  }
  else if (Abstandrechts > Abstandlinks){
    analogWrite(motorTreiberPin3, LinksForward);
    analogWrite(motorTreiberPin2, RechtsBackward);
    Serial.println("Links Ausweichen");
    delay(1000);
    analogWrite(motorTreiberPin3, LinksForward);
    analogWrite(motorTreiberPin1, RechtsForward);
    delay(1000);
  }
  else{
    analogWrite(motorTreiberPin3, LinksForward);
    analogWrite(motorTreiberPin2, RechtsBackward);
    delay(1000);
    Serial.println("180° Drehung");
  }
}


void fahren(){
  //Motor Control A in both directions
  analogWrite(motorTreiberPin1, speed1);
  //Motor Control B in both directions
  analogWrite(motorTreiberPin3, speed2);
  Serial.println("Fahren");
}

void bremsen(){
  //Motor Control A in both directions
  analogWrite(motorTreiberPin1, 0);
  //Motor Control B in both directions
  analogWrite(motorTreiberPin3, 0);
  Serial.println("Bremsen");
}

void rechts(){
  //Motor Control A in both directions
  analogWrite(motorTreiberPin1, 0);
  //Motor Control B in both directions
  analogWrite(motorTreiberPin3, speed1);
  delay(1000);
  Serial.println("Rechts");
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
