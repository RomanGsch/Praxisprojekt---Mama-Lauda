//US1-Mitte1
const int triggerPinM = 3;
const int echoPinM = 8;

//US2-Links
const int triggerPinL = 4;
const int echoPinL = 9;

//US3-Rechts
const int triggerPinR = 2;
const int echoPinR = 7;

//US4-Mitte2
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

//Messungroutine Ultraschallsensoren - Trigger/Echo
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

//Abstandsvergleich Links/Rechts - Speicherung der Abstandswerte
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
    delay(1000); //Drehung um dem Objekt auszuweichen
    bremsen();
    counter1++;
    Serial.println(counter1);
      if (counter1 < 4){
        Serial.println("Objekt umfahren");
        do{
          delay(100);
          abstM1 = messen(triggerPinM, echoPinM, "Mitte1 messen aktiv - Umfahren");
          abstM2 = messen(triggerPinH, echoPinH, "Mitte2 messen aktiv - Umfahren");
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
    delay(1000); //Drehung um dem Objekt auszuweichen
    bremsen();
    counter2++;
    Serial.println(counter2);
      if (counter2 < 4){
        Serial.println("Objekt umfahren");
        do{
          delay(100);
          abstM1 = messen(triggerPinM, echoPinM, "Mitte1 messen aktiv - Umfahren");
          abstM2 = messen(triggerPinH, echoPinH, "Mitte2 messen aktiv - Umfahren");
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
    delay(30000000);
    Serial.println("Abst1 und Abst2 sind gleich");
  }
}
void rueckwaerts(){
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, speed1);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin4, speed1);
  delay(1300);
  Serial.println("Rückwärts aktiv");
  bremsen();
}
void fahren(){
  analogWrite(motorTreiberPin1, speed1);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, speed1);
  analogWrite(motorTreiberPin4, 0);
  Serial.println("Fahrroutine aktiv");
}

void bremsen(){
  analogWrite(motorTreiberPin1, 0);
  analogWrite(motorTreiberPin2, 0);
  analogWrite(motorTreiberPin3, 0);
  analogWrite(motorTreiberPin4, 0);
  delay(1000);
  Serial.println("Bremsroutine aktiv");
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
  if (digitalRead(start)){ //Startsignal von Basisstation (RaspberryPi)
    abstM1 = messen(triggerPinM, echoPinM, "Mitte1 messen");
    abstlinks = messen(triggerPinL, echoPinL, "Links messen");
    abstrechts = messen(triggerPinR, echoPinR, "Rechts messen");
    abstM2 = messen(triggerPinH, echoPinH, "Mitte2 messen");
    fahren();
    if(abstM1<=35 || abstM2<=35){ //|| abstlinks<=20 || abstrechts<=20
      bremsen();
      abstL = pingLR(triggerPinL, echoPinL, "Links messen - Ping"); //Speicherung des linken Abstandes
      delay(200);
      abstR = pingLR(triggerPinR, echoPinR, "Rechts messen - Ping"); //Speicherung des rechten Abstandes
      delay(200);
      abstVgl(abstL, abstR); //Abstandsvergleichs-routine
    }
    else{
    fahren();
    counter1 = 0; //Rücksätzen Zähler1
    counter2 = 0; //Rücksetzen Zähler2
    }
  }
  else{
    bremsen(); //Stillstand (Bremsen) wenn Startsignal "0" ist
  }
}
