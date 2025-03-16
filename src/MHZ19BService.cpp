#include "MHZ19BService.h"

MHZ19BService::MHZ19BService(SoftwareSerial* serial) 
  : mhzSerial(serial) {
  // Use the externally managed SoftwareSerial object
}

MHZ19BService::~MHZ19BService() {
  // Do not delete mhzSerial since it's managed externally
}

void MHZ19BService::begin() {
  Serial.begin(9600);  // For debugging
  Serial.println("MH-Z19B Service Started");
}

bool MHZ19BService::readMeasurement(int& co2) {
  byte mhzResponse[9];

  // Flush any leftover data
  while (mhzSerial->available()) {
    mhzSerial->read();
  }

  // Send CO2 read command
  mhzSerial->write(mhzCmd, 9);
  delay(100);  // Wait for response

  if (mhzSerial->available() >= 9) {
    for (int i = 0; i < 9; i++) {
      mhzResponse[i] = mhzSerial->read();
    }

    // Verify response header
    if (mhzResponse[0] == 0xFF && mhzResponse[1] == 0x86) {
      // Calculate checksum
      byte checksum = 0;
      for (int i = 1; i < 8; i++) {
        checksum += mhzResponse[i];
      }
      checksum = 0xFF - checksum + 1;

      if (checksum == mhzResponse[8]) {
        // Calculate and return CO2 in ppm
        co2 = mhzResponse[2] * 256 + mhzResponse[3];
        return true;  // Success
      } else {
        Serial.println("MH-Z19B checksum error");
        return false;  // Failure
      }
    } else {
      Serial.println("MH-Z19B response error");
      return false;  // Failure
    }
  } else {
    Serial.println("MH-Z19B no response");
    return false;  // Failure
  }
}