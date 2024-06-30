#include <Arduino.h>
#include <GyverBME280.h>
#include "Lcd1602Display.h"
#include "MhZ19B.h"

#define STRING(Value) #Value

Lcd1602Display lcd1602Display;
MhZ19B mhZ19B;
GyverBME280 bme;

void setup()
{
  lcd1602Display.init();
  mhZ19B.init();
  bme.begin();
}

void loop()
{
  delay(5000);
  float bmeHumidity = bme.readHumidity();
  float bmeTemperature = bme.readTemperature();
  float bmePressure = bme.readPressure() * 0.7500637554192 / 100; // mmHg
  int ppm = mhZ19B.getPPM();

  lcd1602Display.draw(bmeHumidity, bmeTemperature, bmePressure, ppm);
}