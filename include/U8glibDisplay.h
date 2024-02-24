/*
  The circuit:
   Display
 ** SCL A5
 ** SDA A4
 ***Using U8glib.h U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);
*/

#include "U8glib.h"

class U8glibDisplay
{
public:
    U8glibDisplay();
    U8GLIB_SSD1306_128X32 u8g;

    // display SSD1306_128X32
    void draw(float humidity, float temperatureFahrenheit, float heatIndex, int PPM);
    void draw(float bmePressure, int PPM);
};

U8glibDisplay::U8glibDisplay()
{
    this->u8g = (U8G_I2C_OPT_NONE);
}

void U8glibDisplay::draw(float humidity, float temperature, float heatIndex, int PPM)
{
    u8g.firstPage();
    do
    {
        // print sensor value
        u8g.setFont(u8g_font_timB10r);
        u8g.setPrintPos(0, 12);
        String hum = "h: ";
        hum.concat(humidity);
        u8g.print(hum);

        u8g.setFont(u8g_font_timB10r);
        u8g.setPrintPos(60, 12);
        String temp = "t: ";
        temp.concat(temperature);
        u8g.print(temp);

        u8g.setFont(u8g_font_timB10r);
        u8g.setPrintPos(0, 32);
        String htIx = "t: ";
        htIx.concat(heatIndex);
        u8g.print(htIx);

        u8g.setFont(u8g_font_timB10r);
        u8g.setPrintPos(60, 32);
        String ppm = "PPM: ";
        ppm.concat(PPM);
        u8g.print(ppm);

    } while (u8g.nextPage());
};

void U8glibDisplay::draw(float bmePressure, int PPM)
{
    u8g.firstPage();
    do
    {
        // print sensor value
        u8g.setFont(u8g_font_timB10r);
        u8g.setPrintPos(30, 12);
        String pressure = "p: ";
        pressure.concat(bmePressure);
        u8g.print(pressure);

        u8g.setFont(u8g_font_timB10r);
        u8g.setPrintPos(30, 32);
        String ppm = "PPM: ";
        ppm.concat(PPM);
        u8g.print(ppm);

    } while (u8g.nextPage());
};