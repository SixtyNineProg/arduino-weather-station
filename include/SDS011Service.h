#ifndef SDS011SERVICE_H
#define SDS011SERVICE_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class SDS011Service {
private:
  SoftwareSerial* sdsSerial;  // Pointer to an external SoftwareSerial object
  
  // Command constants
  const uint8_t CMD_HEADER = 0xAA;
  const uint8_t CMD_TAIL = 0xAB;
  const uint8_t CMD_ID = 0xB4;
  const uint8_t CMD_SET_PERIOD = 0x08;
  const uint8_t CMD_SET_MODE = 0x06;

public:
  SDS011Service(SoftwareSerial* serial);  // Constructor takes SoftwareSerial pointer
  ~SDS011Service();  // Destructor
  
  void begin();
  bool setWorkingPeriod(uint8_t period);
  bool setSleepMode(bool sleep);
  bool readMeasurement(float& pm25, float& pm10);
};

#endif