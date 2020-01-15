int LED = xx;
int brightness = 255;
int dunkel = 0;


void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  if (fahren == 1 or bremsen == 1){
    analogWrite(LED, brightness);
    Serial.print("LED Ein");
  }
  else {
    analogWrite(LED, dunkel);
    Serial.print("LED Aus");
  }
}
