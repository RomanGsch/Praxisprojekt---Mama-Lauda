String incoming = ""; // for incoming serial data

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(10, OUTPUT);
}

void loop() {
  // send data only when you receive data:
  Serial.print("Hello From Arduino!");
  if (Serial.available() > 0) {
    // read the incoming byte:
    incoming = Serial.read();

    if(incoming=="Hallo"){
      digitalWrite(10, HIGH);
    } else {
      digitalWrite(10, LOW);
    }
    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(incoming);
  }
  delay(1000);
}
