//US1-Mitte
const int triggerPinM = 3;
const int echoPinM = 8;

//US2-Links
const int triggerPinL = 4;
const int echoPinL = 9;

//US3-Rechts
const int triggerPinR = 2;
const int echoPinR = 7;

//Motor 1
const int motorTreiberPin1 = 5; //motorTreiberPin1 -> ArduinoOut5
const int motorTreiberPin2  = 6; //motorTreiberPin2 -> ArduinoOut6

//Motor 2
const int motorTreiberPin3  = 11;//motorTreiberPin3 -> ArduinoOut10
const int motorTreiberPin4  = 10; //motorTreiberPin4 -> ArduinoOut11

long dauer, abst, abstLR, abstL, abstR;

int speed1 = 255;
//int speed2 = 200; //eventuell erst benötigt wenn ausgleich der beiden seiten sein muss (gerade aus fahren)

//const int RechtsForward = 200; //max Speed soll lt. Datenblatt bei 180 sein
//const int RechtsBackward = 200;
//const int LinksForward = 200;
//const int LinksBackward = 200;

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
    analogWrite(motorTreiberPin3, 0);
    analogWrite(motorTreiberPin4, speed1);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin1, speed1);
    Serial.println("Rechts Ausweichen");
    delay(2000);
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed1);
    delay(2000);
  }
  else if (abstR > abstL){
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed1);
    Serial.println("Links Ausweichen");
    delay(2000);
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin2, 0);
    analogWrite(motorTreiberPin1, speed1);
    delay(2000);
  }
  else{
    analogWrite(motorTreiberPin4, 0);
    analogWrite(motorTreiberPin3, speed1);
    analogWrite(motorTreiberPin1, 0);
    analogWrite(motorTreiberPin2, speed1);
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
  fahren();
  if(abst<=30){
    bremsen();
    delay(100);
    abstL = pingLR(triggerPinL, echoPinL, "Links");
    delay(100);
    abstR = pingLR(triggerPinR, echoPinR, "Rechts");
    delay(100);
    abstVgl(abstL, abstR);
  }
  
  else{
    fahren();
    
  }
}
