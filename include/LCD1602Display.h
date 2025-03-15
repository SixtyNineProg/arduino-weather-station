/*
  The circuit:
   Display
 ** SCL A5
 ** SDA A4
*/

#include <Wire.h>              // Подключение библиотеки для I2C
#include <LiquidCrystal_I2C.h> // Подключение библиотеки для управления LCD1602

#define STRING(Value) #Value

class Lcd1602Display
{
public:
    Lcd1602Display();
    LiquidCrystal_I2C lcd;

    void init();
    void Lcd1602Display::draw(float bmeHumidity, float bmeTemperature, float bmePressure, int ppm, float pm25, float pm10);
};

// адрес дисплея 0x3f или 0x27
// размер дисплея 16x2 (поддерживаются и другие, например 20x4)
// адрес, столбцов, строк
Lcd1602Display::Lcd1602Display() : lcd(0x27, 16, 2)
{
}

void Lcd1602Display::init()
{
    lcd.init();
    lcd.backlight();
}

void Lcd1602Display::draw(float bmeHumidity,
                          float bmeTemperature,
                          float bmePressure,
                          int ppm,
                          float pm25,
                          float pm10)
{
    lcd.setCursor(0, 0);
    lcd.print(String(bmeHumidity));
    lcd.setCursor(0, 1);
    lcd.print(String(bmeTemperature));
    lcd.setCursor(6, 0);
    lcd.print(String(round(bmePressure)));
    lcd.setCursor(6, 1);
    lcd.print(String(ppm));
    lcd.setCursor(10, 0);
    lcd.print(String(pm25));
    lcd.setCursor(10, 1);
    lcd.print(String(pm10));
};