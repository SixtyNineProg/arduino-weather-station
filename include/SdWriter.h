#include <SPI.h>
#include <SD.h>

#define ChipSelect 10

const String GREETING = "Start";

class SdWriter
{
private:
  /* data */
public:
  SdWriter(/* args */);
  ~SdWriter();
  void initSd();
  void printToSdCard(float humidity, float temperature, float heatIndex, int PPM);
};

SdWriter::SdWriter(/* args */)
{
}

SdWriter::~SdWriter()
{
}

void SdWriter::initSd()
{
  Serial.print("Initializing SD card...");
  while (1)
  {
    if (!SD.begin(ChipSelect))
    {
      Serial.println("Card failed, or not present");
    }
    else
      break;
    delay(5000);
  }
  Serial.println("card initialized.");
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println(GREETING);
  dataFile.close();
  Serial.println("init SD OK");
}

void SdWriter::printToSdCard(float humidity, float temperature, float heatIndex, int PPM)
{
  while (1)
  {
    if (!SD.begin(ChipSelect))
    {
      Serial.println("Card failed, or not present");
    }
    else
      break;
    delay(5000);
  }

  File dataFile = SD.open("DATALOG.txt", FILE_WRITE);
  if (dataFile != 0)
  {
    dataFile.print(humidity);
    dataFile.print('\t');
    dataFile.print(temperature);
    dataFile.print('\t');
    dataFile.print(heatIndex);
    dataFile.print('\t');
    dataFile.println(PPM);
    dataFile.close();
  }
  else
  {
    Serial.println("error opening datalog.txt");
  }
}
