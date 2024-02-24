#include <Arduino.h>
#include <GyverBME280.h>
#include "U8glibDisplay.h"
#include "Lcd1602Display.h"
#include "DhtSensor.h"
#include "MhZ19B.h"
#include "Sd.h"

#define STRING(Value) #Value

U8glibDisplay u8glibdisplay;
Lcd1602Display lcd1602Display;
DhtSensor dhtSensor;
MhZ19B mhZ19B;
GyverBME280 bme;

void setup()
{
  lcd1602Display.init();
  dhtSensor.init();
  mhZ19B.init();
  bme.begin();
}

void loop()
{
  delay(1000);
  float dhtHumidity = dhtSensor.getHumidity();
  float dhtTemperature = dhtSensor.getTemperature();
  float bmeHumidity = bme.readHumidity();
  float bmeTemperature = bme.readTemperature();
  float bmePressure = bme.readPressure();
  int ppm = mhZ19B.getPPM();

  u8glibdisplay.draw(bmePressure, ppm);
  lcd1602Display.draw(dhtHumidity, dhtTemperature, bmeHumidity, bmeTemperature, bmePressure, ppm);
}