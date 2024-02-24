#include <MHZ19_uart.h>
MHZ19_uart mhz19;

#define MHZ_RX 2
#define MHZ_TX 3

#define CO2_MIN 300
#define CO2_MAX 2000

class MhZ19B
{
public:
    MhZ19B();
    ~MhZ19B();
    int getPPM();
    void init();
};

MhZ19B::MhZ19B()
{
}

MhZ19B::~MhZ19B()
{
}

int MhZ19B::getPPM()
{
    return mhz19.getPPM();
}

void MhZ19B::init()
{
    mhz19.begin(MHZ_TX, MHZ_RX);
    mhz19.setAutoCalibration(true);
    mhz19.getStatus();
    delay(500);
    if (mhz19.getStatus() == 0)
    {
        Serial.println(F("MhZ19B OK"));
    }
    else
    {
        Serial.println(F("MhZ19B ERROR"));
    }
}
