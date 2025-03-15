#include <Arduino.h>
#include <GyverBME280.h>
#include "Lcd1602Display.h"
#include <SoftwareSerial.h>

#define STRING(Value) #Value

SoftwareSerial sdsSerial(3, 4); // RX, TX for SDS011
SoftwareSerial mhzSerial(5, 6); // RX, TX for MH-Z19B

Lcd1602Display lcd1602Display;
GyverBME280 bme;

// MH-Z19B command to read CO2
byte mhzCmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

bool readMHZ19B(int *co2Value);
bool readSDS011(float *pm25Value, float *pm10Value);

void setup()
{
  Serial.begin(9600);
  lcd1602Display.init();
  bme.begin();
  delay(3000);
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

  // Read SDS011 and update pm25, pm10
  sdsSerial.begin(9600);
  bool resultSds = readSDS011(&pm25, &pm10);
  Serial.println("Result SDS: " + String(resultSds));
  sdsSerial.end();

  // Read MH-Z19B and update co2
  mhzSerial.begin(9600);
  bool resultMhz = readMHZ19B(&co2);
  Serial.println("Result MHZ: " + String(resultMhz));
  mhzSerial.end();

  lcd1602Display.draw(bmeHumidity, bmeTemperature, bmePressure, co2, pm25, pm10);
  delay(5000); // Wait 5 seconds between readings
}

// Function to read SDS011 and return PM2.5 and PM10 via pointers
bool readSDS011(float *pm25Value, float *pm10Value)
{
  byte sdsData[10];
  unsigned long timeout = millis() + 1000; // 1-second timeout

  // Flush any leftover data
  while (sdsSerial.available())
    sdsSerial.read();

  // Wait for a valid packet
  while (millis() < timeout)
  {
    if (sdsSerial.available() >= 10)
    {
      if (sdsSerial.read() == 0xAA)
      { // Check header
        if (sdsSerial.read() == 0xC0)
        { // Check command ID
          sdsData[0] = 0xAA;
          sdsData[1] = 0xC0;
          for (int i = 2; i < 10; i++)
          {
            sdsData[i] = sdsSerial.read();
          }

          // Verify checksum and tail
          byte checksum = 0;
          for (int i = 2; i < 8; i++)
          {
            checksum += sdsData[i];
          }
          if (checksum == sdsData[8] && sdsData[9] == 0xAB)
          {
            // Calculate and return PM values
            *pm25Value = (sdsData[2] + sdsData[3] * 256) / 10.0;
            *pm10Value = (sdsData[4] + sdsData[5] * 256) / 10.0;
            return true; // Success
          }
          else
          {
            Serial.println("SDS011 checksum or tail error");
            return false; // Failure
          }
        }
      }
    }
  }
  Serial.println("SDS011 timeout - no valid data");
  return false; // Failure
}

// Function to read MH-Z19B and return CO2 via pointer
bool readMHZ19B(int *co2Value)
{
  byte mhzResponse[9];

  // Flush any leftover data
  while (mhzSerial.available())
    mhzSerial.read();

  // Send CO2 read command
  mhzSerial.write(mhzCmd, 9);
  delay(100); // Wait for response

  if (mhzSerial.available() >= 9)
  {
    for (int i = 0; i < 9; i++)
    {
      mhzResponse[i] = mhzSerial.read();
    }

    // Verify response header
    if (mhzResponse[0] == 0xFF && mhzResponse[1] == 0x86)
    {
      // Calculate checksum
      byte checksum = 0;
      for (int i = 1; i < 8; i++)
      {
        checksum += mhzResponse[i];
      }
      checksum = 0xFF - checksum + 1;

      if (checksum == mhzResponse[8])
      {
        // Calculate and return CO2 in ppm
        *co2Value = mhzResponse[2] * 256 + mhzResponse[3];
        return true; // Success
      }
      else
      {
        Serial.println("MH-Z19B checksum error");
        return false; // Failure
      }
    }
    else
    {
      Serial.println("MH-Z19B response error");
      return false; // Failure
    }
  }
  else
  {
    Serial.println("MH-Z19B no response");
    return false; // Failure
  }
}