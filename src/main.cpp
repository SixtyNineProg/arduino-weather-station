#include <Arduino.h>
#include <GyverBME280.h>
#include "Lcd1602Display.h"
#include <SoftwareSerial.h>
#include "SDS011Service.h"
#include "MHZ19BService.h"

#define STRING(Value) #Value

SoftwareSerial sdsSerial(4, 5); // RX, TX for SDS011
SoftwareSerial mhzSerial(2, 3); // RX, TX for MH-Z19B

// Sensor service instances
SDS011Service sdsSensor(&sdsSerial);
MHZ19BService mhzSensor(&mhzSerial);

Lcd1602Display lcd1602Display;
GyverBME280 bme;

void setup()
{
  Serial.begin(9600);
  lcd1602Display.init();
  bme.begin();

  // Initialize SDS011
  sdsSerial.begin(9600);
  sdsSensor.begin();
  sdsSensor.setWorkingPeriod(0); // Set to continuous mode
  sdsSensor.setSleepMode(false); // Wake up sensor

  // Initialize MH-Z19B (no specific setup needed beyond serial)
  mhzSensor.begin();

  delay(5000); // warm-up for SDS011 fan
}

void loop()
{
  delay(5000);
  float bmeHumidity = bme.readHumidity();                         // percent
  float bmeTemperature = bme.readTemperature();                   // degrees Celsius
  float bmePressure = bme.readPressure() * 0.7500637554192 / 100; // mmHg

  float pm25 = 0.0; // PM2.5 in µg/m³
  float pm10 = 0.0; // PM10 in µg/m³
  int co2 = 0;      // CO2 in ppm

  // Re-enable SDS011 serial after MH-Z19B is done
  sdsSerial.begin(9600);
  // Read SDS011 using the service (sdsSerial is already active)
  bool resultSds = sdsSensor.readMeasurement(pm25, pm10);
  Serial.println("Result SDS: " + String(resultSds));
  if (resultSds)
  {
    Serial.print("PM2.5: ");
    Serial.print(pm25);
    Serial.println(" µg/m³");
    Serial.print("PM10: ");
    Serial.print(pm10);
    Serial.println(" µg/m³");
  }
  // Disable SDS011 serial before activating MH-Z19B serial
  sdsSerial.end();

  // Read MH-Z19B using the service
  mhzSerial.begin(9600);
  bool resultMhz = mhzSensor.readMeasurement(co2);
  Serial.println("Result MHZ: " + String(resultMhz));
  if (resultMhz)
  {
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.println(" ppm");
  }
  mhzSerial.end();

  lcd1602Display.draw(bmeHumidity, bmeTemperature, bmePressure, co2, pm25, pm10);
  delay(5000); // Wait 5 seconds between readings
}