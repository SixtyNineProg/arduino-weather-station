#include <Arduino.h>
#include <GyverBME280.h>
#include "Lcd2004Display.h"
#include <SoftwareSerial.h>
#include "SDS011Service.h"
#include "MHZ19BService.h"

// SoftwareSerial connections for sensors
SoftwareSerial sdsSerial(4, 5); // RX, TX for SDS011
SoftwareSerial mhzSerial(2, 3); // RX, TX for MH-Z19B

// Sensor service instances
SDS011Service sdsSensor(&sdsSerial);
MHZ19BService mhzSensor(&mhzSerial);

Lcd2004Display lcd2004Display;
GyverBME280 bme;

float bmeHumidity = 0.0;
float bmeTemperature = 0.0;
float bmePressure = 0.0;
int co2 = 0;
float pm25 = 0.0;
float pm10 = 0.0;

unsigned long lastSdsReadTime = 0;    // Time of the last SDS011 reading
unsigned long sdsWarmupStartTime = 0; // Time when SDS011 warmup started
bool isSdsWarmingUp = false;          // Flag indicating warmup in progress
bool isFirstSdsRead = true;           // Flag for initial SDS011 reading
const unsigned long SDS_READ_INTERVAL = 600000;
const unsigned long SDS_WARMUP_TIME = 30000;

void setup()
{
  Serial.begin(9600);
  lcd2004Display.init();
  bme.begin();

  // Initialize MH-Z19B
  mhzSensor.begin();

  delay(3000);

  // Initialize SDS011
  sdsSerial.begin(9600);
  sdsSensor.begin();
  sdsSensor.setWorkingPeriod(0); // Set to continuous mode
  sdsSensor.setSleepMode(true);  // Put into sleep mode initially
  sdsSerial.end();

  delay(3000);

  // Start initial SDS011 warmup immediately
  sdsSerial.begin(9600);
  sdsSensor.setSleepMode(false); // Wake up the sensor
  sdsSerial.end();
  sdsWarmupStartTime = millis();
  isSdsWarmingUp = true;
  Serial.println("SDS011 started initial warmup");
}

void readBme280()
{
  bmeHumidity = bme.readHumidity();                         // Humidity, %
  bmeTemperature = bme.readTemperature();                   // Temperature, °C
  bmePressure = bme.readPressure() * 0.7500637554192 / 100; // Pressure, mmHg
}

void readMhz19b()
{
  mhzSerial.begin(9600);
  bool resultMhz = mhzSensor.readMeasurement(co2);
  mhzSerial.end();
  if (resultMhz)
  {
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.println(" ppm");
  }
  else
  {
    Serial.println("Error reading MH-Z19B");
  }
}

void manageSds011()
{
  unsigned long currentTime = millis();

  // Handle warmup and reading (both initial and periodic)
  if (isSdsWarmingUp && (currentTime - sdsWarmupStartTime >= SDS_WARMUP_TIME))
  {
    sdsSerial.begin(9600);
    bool resultSds = sdsSensor.readMeasurement(pm25, pm10);
    if (resultSds)
    {
      Serial.print("PM2.5: ");
      Serial.print(pm25);
      Serial.println(" µg/m³");
      Serial.print("PM10: ");
      Serial.print(pm10);
      Serial.println(" µg/m³");
    }
    else
    {
      Serial.println("Error reading SDS011");
    }
    sdsSensor.setSleepMode(true);
    sdsSerial.end();
    isSdsWarmingUp = false;
    lastSdsReadTime = currentTime; // Update last read time
    if (isFirstSdsRead)
    {
      Serial.println("SDS011 completed initial reading");
      isFirstSdsRead = false;
    }
    else
    {
      Serial.println("SDS011 completed periodic reading");
    }
  }

  // Start periodic reading (after initial read is complete)
  if (!isFirstSdsRead && !isSdsWarmingUp && (currentTime - lastSdsReadTime >= SDS_READ_INTERVAL))
  {
    sdsSerial.begin(9600);
    sdsSensor.setSleepMode(false);
    sdsWarmupStartTime = currentTime;
    isSdsWarmingUp = true;
    Serial.println("SDS011 started periodic warmup");
  }
}

void loop()
{
  readBme280();
  readMhz19b();
  manageSds011();

  lcd2004Display.draw(bmeHumidity, bmeTemperature, (int)round(bmePressure), co2, pm25, pm10);

  delay(5000); // Pause between iterations
}