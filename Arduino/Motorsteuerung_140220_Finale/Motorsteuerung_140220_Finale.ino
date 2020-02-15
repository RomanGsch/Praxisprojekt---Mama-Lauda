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

long dauer, abst, abstLR, abstL, abstR, abstlinks, abstrechts, abstM1, abstM2;

//Startsignal RaspPI
char start = A0;

//Geschwindigkeitseinstellung
int speed1 = 200; //Fahren
int speed2 = 255; //Drehen

int counter1 =0;
int counter2 =0;

//Messungroutine Ultraschallsensoren
long messen(int triggerPin, int echoPin, String Richtung){
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(3);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  dauer = pulseIn(echoPin, HIGH);
  abst = (dauer/2)/25;
  Serial.println("Richtung: "+Richtung+"/Abstand: "+abst);
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
    counter1++;
    Serial.println(counter1);
      if (counter1 < 4){
        Serial.println("Objekt umfahren");
        do{
          delay(100);
          abstM1 = messen(triggerPinM, echoPinM, "Mitte");
          abstM2 = messen(triggerPinH, echoPinH, "HintenjetzteVorne");
          analogWrite(motorTreiberPin1, speed1);
          analogWrite(motorTreiberPin2, 0);
          analogWrite(motorTreiberPin3, speed1);
          analogWrite(motorTreiberPin4, 0);
        }while(abstM1>=35);
        bremsen();
       }
     else{
        rueckwaerts();
        analogWrite(motorTreiberPin4, speed2);
        analogWrite(motorTreiberPin3, 0);
        analogWrite(motorTreiberPin1, speed2);
        analogWrite(motorTreiberPin2, 0);
        delay(600);
        Serial.println("180° Drehung");
        bremsen();
        counter1 = 0;
        }
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
    counter2++;
    Serial.println(counter2);
      if (counter2 < 4){
        Serial.println("Objekt umfahren");
        do{
          delay(100);
          abstM1 = messen(triggerPinM, echoPinM, "Mitte");
          abstM2 = messen(triggerPinH, echoPinH, "HintenjetzteVorne");
          analogWrite(motorTreiberPin1, speed1);
          analogWrite(motorTreiberPin2, 0);
          analogWrite(motorTreiberPin3, speed1);
          analogWrite(motorTreiberPin4, 0);
        }while(abstM2 >=35);
        bremsen();
      }
      else{
        rueckwaerts();
        analogWrite(motorTreiberPin4, 0);
        analogWrite(motorTreiberPin3, speed2);
        analogWrite(motorTreiberPin1, 0);
        analogWrite(motorTreiberPin2, speed2);
        delay(600);
        Serial.println("Drehung");
        bremsen();
        counter2 = 0;
        }
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
    delay(30000);
    Serial.println("180° Drehung");
  }
}
void rueckwaerts(){
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, speed1);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin4, speed1);
  delay(1300);
  Serial.println("Rückwärts");
  bremsen();
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
  if (digitalRead(start)){
    abstM1 = messen(triggerPinM, echoPinM, "Mitte messen");
    abstlinks = messen(triggerPinL, echoPinL, "Links");
    abstrechts = messen(triggerPinR, echoPinR, "Rechts");
    abstM2 = messen(triggerPinH, echoPinH, "HintenjetzteVorne");
    fahren();
    if(abstM1<=35 || abstM2<=35){ //|| abstlinks<=20 || abstrechts<=20
      bremsen();
      abstL = pingLR(triggerPinL, echoPinL, "Links messen - Ping");
      delay(200);
      abstR = pingLR(triggerPinR, echoPinR, "Rechts messen - Ping");
      delay(200);
      abstVgl(abstL, abstR);
    }
    else{
    fahren();
    counter1 = 0;
    counter2 = 0;
    }
  }
  else{
    bremsen();
  }
}
