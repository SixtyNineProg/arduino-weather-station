#ifndef MHZ19BSERVICE_H
#define MHZ19BSERVICE_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class MHZ19BService {
private:
  SoftwareSerial* mhzSerial;  // Pointer to an external SoftwareSerial object
  
  // MH-Z19B command to read CO2
  const byte mhzCmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};

public:
  MHZ19BService(SoftwareSerial* serial);  // Constructor takes SoftwareSerial pointer
  ~MHZ19BService();  // Destructor
  
  void begin();
  bool readMeasurement(int& co2);  // Read CO2 in ppm
};

#endif