/*
Arduino Tutorial 3.3 - Temperaturgesteuerter Lüfter mit 10kO NTC

In diesem Szenario geht es darum, die Lüftergeschwindigkeit über die Temperatur zu steuern. 
Bei steigender Temperatur, soll auch die Lüftergeschwindigkeit steigen, bei sinkender Temperatur fallen.
*/

// Konstanten
const int fanPin = 9;                 // Pin für den Lüfter
const int ntc = A0;                   // Pin für den 10kO NTC Wiederstand
const int ntcNominal = 10000;         // Wiederstand des NTC bei Nominaltemperatur
const int tempNominal = 25;           // Temperatur bei der der NTC den angegebenen Wiederstand hat
const int bCoefficient = 3977;        // Beta Coefficient(B25 aus Datenblatt des NTC)
const int serienWiederstand = 10000;  // Wert des Wiederstands der mit dem NTC in Serie geschalten ist
const int abfrageZahl = 5;            // Je mehr abfragen, desto stabiler isr das Ergebnis, dauert aber länger

// Variablen
int fanSpeed = 0;          // Variable für die Lüftergeschwindigkeit
int fanMin = 60;           // Kleinster PWM Wert für den Lüfter befor er abschält
int fanOut = 1;            // Variable zum pürfen ob der Lüfter aus war
int tMin = 20;             // Untere Grenze des Temperaturbereichs
int tMax = 60;             // Obere Grenze des Temperaturbereichs
int abfrage[abfrageZahl];  // Array Variable für das Mitteln der Temperatur
float durchschnitt = 0;    // Variable für das Mitteln der Temperatur
float temp;                // Variable für die Berechnung der temperatur nach Steinhart


void setup()
{
  TCCR1B = TCCR1B & 0b11111000 | 0x01;   // Setzt Timer1 (Pin 9 und 10) auf 31300Hz
  Serial.begin(9600);             // Baudrate für die Ausgabe am Serial Monitor
  pinMode(fanPin, OUTPUT);        // Setzt den Pin des Lüfters als Ausgang
  pinMode(ntc, INPUT);            // Setzt den Pin des NTC Wiederstands als Eingang
}


void loop()
{
  temperaturberechnung();      // Startet die Temperaturerfassungsroutine
  
  // Lüftergeschwindigkeit über den Temperaturbereich einstellen
  // TMin->0% PWM | TMax->100% PWM
  fanSpeed = map(temp, tMin, tMax, 0, 255);    
  
  // Wenn der PWM Wert unter den van FanMin fällt, schält der Lüfter ab
  if (fanSpeed < fanMin)
  {
    fanSpeed = 0;
    fanOut = 1;
  }
  
  // Hysterese
  if (fanOut == 1)
  {
    fanSpeed = 0;
  }
  
  if(temp >= 32)
  {
    if(fanOut == 1)
    {
      fanOut = 0;
      analogWrite(fanPin, 255);
    }
  }
  
  // PWM Wert auf 255 begerenzen  
  if (fanSpeed > 255)
  { 
    fanSpeed = 255;
  }
  
  // Lüftergeschwindigkeit über den Seriellen Monitor ausgeben
  Serial.print("Lueftergeschwindigkeit: ");          
  Serial.println(fanSpeed);

  analogWrite(fanPin, fanSpeed);      // Den Lüfter mit dem PWM Wert ansteuern
  delay(500);  
} 



void temperaturberechnung()
{
  // Nimmt N Abfragen in einer Reihe, mit einem kurzen delay
  for (int i=0; i < abfrageZahl; i++)
  {
    abfrage[i] = analogRead(ntc);
    delay(10);
  }
  
  // Mittelt alle Abfragen
  durchschnitt = 0;
  for (int i=0; i < abfrageZahl; i++)
  {
    durchschnitt += abfrage[i];
  }
  durchschnitt /= abfrageZahl;
  
  // Umwandlung des Wertes in Widerstand
  durchschnitt = 1023 / durchschnitt - 1;
  durchschnitt = serienWiederstand / durchschnitt;
  
  // Umrechnung aller Ergebnisse in die Temperatur mittels einer Steinhard Berechnung
  temp = durchschnitt / ntcNominal;     // (R/Ro)
  temp = log(temp);                     // ln(R/Ro)
  temp /= bCoefficient;                 // 1/B * ln(R/Ro)
  temp += 1.0 / (tempNominal + 273.15); // + (1/To)
  temp = 1.0 / temp;                    // Invertieren
  temp -= 273.15;                       // Umwandeln in °C
  
  // Ausgabe an den Seriellen Monitor
  Serial.print("Temperatur ");
  Serial.print(temp);
  Serial.println(" *C");
  
  delay(500);
}
