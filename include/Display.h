/*
  The circuit:
   Display
 ** SCL A5
 ** SDA A4
 ***Using U8glib.h U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);
*/

#include "U8glib.h"

class Display
{
public:
    Display();
    ~Display();
    U8GLIB_SSD1306_128X32 u8g;

    // display SSD1306_128X32
    void draw(float humidity, float temperatureFahrenheit, float heatIndex, int PPM);
};

Display::Display()
{
    this->u8g = (U8G_I2C_OPT_NONE);
}

Display::~Display()
{
}

void Display::draw(float humidity, float temperature, float heatIndex, int PPM)
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