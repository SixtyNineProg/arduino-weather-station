#include "SDS011Service.h"

SDS011Service::SDS011Service(SoftwareSerial* serial) 
  : sdsSerial(serial) {
  // Use the externally managed SoftwareSerial object
}

SDS011Service::~SDS011Service() {
  // Do not delete sdsSerial since it's managed externally
}

void SDS011Service::begin() {
  Serial.begin(9600);  // For debugging
  Serial.println("SDS011 Service Started");
}

bool SDS011Service::setWorkingPeriod(uint8_t period) {
  if (period > 30) {
    Serial.println("Error: Period must be 0-30 minutes");
    return false;
  }

  uint8_t cs = 7 + period;  // Checksum (simplified from your example)
  uint8_t cmd[19] = {
    CMD_HEADER,    // 0xAA
    CMD_ID,        // 0xB4
    CMD_SET_PERIOD,// 0x08
    0x01,          // Active mode
    period,        // 0 for continuous, 1-30 for minutes
    0x00, 0x00, 0x00, 0x00, 0x00, // Fill bytes
    0x00, 0x00, 0x00, 0x00, 0x00, // Fill bytes
    0xFF, 0xFF,    // Device ID
    cs,            // Checksum
    CMD_TAIL       // 0xAB
  };

  for (uint8_t i = 0; i < 19; i++) {
    sdsSerial->write(cmd[i]);
  }

  unsigned long startTime = millis();
  while (millis() - startTime < 1000) {
    if (sdsSerial->available() >= 10) {
      uint8_t response[10];
      for (int i = 0; i < 10; i++) {
        response[i] = sdsSerial->read();
      }
      if (response[0] == 0xAA && response[1] == 0xC5) {
        Serial.print("Working period set to ");
        Serial.print(period);
        Serial.println(" minutes");
        return true;
      }
    }
  }
  Serial.println("Warning: No valid response from sensor (period may still be set)");
  return true;  // Assume success if command was sent
}

bool SDS011Service::setSleepMode(bool sleep) {
  uint8_t cmd[19];
  if (sleep) {
    uint8_t sleep_command[] = {
      0xAA, 0xB4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0xAB
    };
    memcpy(cmd, sleep_command, 19);
  } else {
    uint8_t wakeup_command[] = {
      0xAA, 0xB4, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0xAB
    };
    memcpy(cmd, wakeup_command, 19);
  }

  // Send command
  for (uint8_t i = 0; i < 19; i++) {
    sdsSerial->write(cmd[i]);
  }

  unsigned long startTime = millis();
  while (millis() - startTime < 2000) {
    if (sdsSerial->available() >= 10) {
      uint8_t response[10];
      for (int i = 0; i < 10; i++) {
        response[i] = sdsSerial->read();
      }
      if (response[0] == 0xAA && response[1] == 0xC5) {
        Serial.println(sleep ? "Sensor set to sleep mode" : "Sensor set to work mode");
        return true;
      }
    }
  }
  Serial.println("Warning: No valid response from sensor (mode may still be set)");
  return true;  // Assume success if command was sent
}

bool SDS011Service::readMeasurement(float& pm25, float& pm10) {
  uint8_t sdsData[10];
  unsigned long timeout = millis() + 1000;  // 1-second timeout

  // Flush any leftover data
  while (sdsSerial->available()) {
    sdsSerial->read();
  }

  // Wait for a valid packet
  while (millis() < timeout) {
    if (sdsSerial->available() >= 10) {
      if (sdsSerial->read() == 0xAA) {  // Check header
        if (sdsSerial->read() == 0xC0) {  // Check command ID
          sdsData[0] = 0xAA;
          sdsData[1] = 0xC0;
          for (int i = 2; i < 10; i++) {
            sdsData[i] = sdsSerial->read();
          }

          // Verify checksum and tail
          uint8_t checksum = 0;
          for (int i = 2; i < 8; i++) {
            checksum += sdsData[i];
          }
          if (checksum == sdsData[8] && sdsData[9] == 0xAB) {
            // Calculate and return PM values
            pm25 = (sdsData[2] + sdsData[3] * 256) / 10.0;
            pm10 = (sdsData[4] + sdsData[5] * 256) / 10.0;
            return true;
          } else {
            Serial.println("SDS011 checksum or tail error");
            return false;
          }
        }
      }
    }
  }
  Serial.println("SDS011 timeout - no valid data");
  return false;
}