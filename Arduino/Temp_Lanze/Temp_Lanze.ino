//Libraries für den Sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp_lanze(&oneWire);

void setup() {
  Serial.begin(9600);
  temp_lanze();
}
void loop() {
  temp_lanze.requestTemperatures();
  Serial.print("Temperatur an Lanze: ");
  Serial.print(temp_lanze.getTempCByIndex(0));
  Serial.println(" °C");
}
