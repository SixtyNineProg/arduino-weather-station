#include "DHT.h"
#include <Adafruit_Sensor.h>

#define DHTPIN 4 // what pin we're connected to
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE, 6);

class DhtSensor
{
public:
    DhtSensor();
    ~DhtSensor();
    float getHumidity();
    float getTemperature();
    float getTemperatureFahrenheit();
    float getHeatIndex(float temperatureFahrenheit, float humidity);
    void initSensor();
};

DhtSensor::DhtSensor()
{
}

DhtSensor::~DhtSensor()
{
}

float DhtSensor::getHumidity()
{
    return dht.readHumidity();
}

float DhtSensor::getTemperature()
{
    return dht.readTemperature();
}

float DhtSensor::getTemperatureFahrenheit()
{
    return dht.readTemperature(true);
}

float DhtSensor::getHeatIndex(float temperatureFahrenheit, float humidity)
{
    return dht.computeHeatIndex(temperatureFahrenheit, humidity);
}

void DhtSensor::initSensor()
{
    dht.begin();
    Serial.println("Dht init successfull");
}
