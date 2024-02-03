#include <Arduino.h>
#include <GyverBME280.h>  
#include "Display.h"
#include "DhtSensor.h"
#include "MhZ19B.h"
#include "Sd.h"

Display display;
DhtSensor dht11;
MhZ19B mhZ19B;
GyverBME280 bme;

void setup()
{
  Serial.begin(9600);
  dht11.initSensor();
  mhZ19B.initSensor();
  bme.begin();
}

void loop()
{
  delay(1000);
  float humidity = dht11.getHumidity();
  float temperature = dht11.getTemperature();
  float bmeTemperature = bme.readTemperature();
  int ppm = mhZ19B.getPPM();

  display.draw(humidity, temperature, bmeTemperature, ppm);
}