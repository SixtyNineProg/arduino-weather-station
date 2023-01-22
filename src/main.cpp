#include <Arduino.h>
#include "Display.h"
#include "DhtSensor.h"
#include "MhZ19B.h"
#include "Sd.h"
#include "SdWriter.h"

Display display;
DhtSensor dht11;
MhZ19B mhZ19B;
SdWriter sdWriter;

void setup()
{
  Serial.begin(9600);
  dht11.initSensor();
  mhZ19B.initSensor();
  sdWriter.initSd();
}

void loop()
{
  delay(10000);
  float humidity = dht11.getHumidity();
  float temperature = dht11.getTemperature();
  float heatIndex = dht11.getHeatIndex(dht11.getTemperatureFahrenheit(), humidity);

  int ppm = mhZ19B.getPPM();

  display.draw(humidity, temperature, heatIndex, ppm);
  sdWriter.printToSdCard(humidity, temperature, heatIndex, ppm);
}