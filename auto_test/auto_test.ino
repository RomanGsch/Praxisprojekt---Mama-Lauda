//Sensor
const int triggerPin = 2;
const int echoPin = 3;
long dauer, abst;

//Motor 1
const int motorPin1 = 9;
//const int motorPin2  = 8;

//Motor 2
const int motorPin3  = 7;
//const int motorPin4  = 6;

int speed1 = 180;
int speed2 = 140;

long messen(){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(3);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  dauer = pulseIn(echoPin, HIGH);
  abst = (dauer/2)/25;
  Serial.println(abst);
  return abst;
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
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop(){
  //abst = messen();
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(3);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  dauer = pulseIn(echoPin, HIGH);
  abst = (dauer/2)/25;
  Serial.println(abst);

  if(abst<=30){
    bremsen();
    delay(1000);
    rechts();
  }
  else{
    fahren();
    
  }
}
