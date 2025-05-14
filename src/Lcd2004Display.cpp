#include "Lcd2004Display.h"

// адрес дисплея 0x3f или 0x27
// размер дисплея 20x4 для LCD2004
// адрес, столбцов, строк
Lcd2004Display::Lcd2004Display() : lcd(0x27, 20, 4)
{
}

void Lcd2004Display::init()
{
    lcd.init();
    lcd.backlight();
}

void Lcd2004Display::draw(float bmeHumidity, float bmeTemperature, int bmePressure, int ppm, float pm25, float pm10)
{
    // Row 0: Humidity and Temperature
    lcd.setCursor(0, 0);
    lcd.print("H:");
    lcd.print(bmeHumidity, 1);
    lcd.print("%");
    lcd.setCursor(10, 0);
    lcd.print("T:");
    lcd.print(bmeTemperature, 1);
    lcd.print("C");

    // Row 1: Pressure and CO2
    lcd.setCursor(0, 1);
    lcd.print("P:");
    lcd.print(bmePressure);
    lcd.print("hPa");
    lcd.setCursor(10, 1);
    lcd.print("CO2:");
    lcd.print(ppm);
    lcd.print("ppm");

    // Row 2: PM2.5
    lcd.setCursor(0, 2);
    lcd.print("PM2.5: ");
    lcd.print(pm25, 1);
    lcd.print(" ug/m3");

    // Row 3: PM10
    lcd.setCursor(0, 3);
    lcd.print("PM10: ");
    lcd.print(pm10, 1);
    lcd.print(" ug/m3");
}