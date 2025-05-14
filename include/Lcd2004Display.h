#ifndef LCD2004_DISPLAY_H
#define LCD2004_DISPLAY_H

#include <Wire.h>              // Подключение библиотеки для I2C
#include <LiquidCrystal_I2C.h> // Подключение библиотеки для управления LCD

#define STRING(Value) #Value

class Lcd2004Display
{
public:
    LiquidCrystal_I2C lcd;
    Lcd2004Display();
    void init();
    void draw(float bmeHumidity, float bmeTemperature, int bmePressure, int ppm, float pm25, float pm10);
};

#endif