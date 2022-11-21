#include "btcomms.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define BT_DEBUG

BTComms::BTComms(const char *localName, Leds &leds)
    : m_localName(localName), m_BT(), m_leds(leds)
{
}

void BTComms::init()
{
    m_BT.begin(m_localName);
#ifdef BT_DEBUG
    Serial.print("You can now pair with your device");
#endif
}

void BTComms::write()
{
    if (Serial.available()) 
    {
        m_BT.write(Serial.read());
    }
}

void BTComms::read()
{
    if (m_BT.available()) 
    {
        if(m_BT.read() == 'M')
        {
            m_leds.setEffect(m_leds.getEffect() + 1);
        }
    }
}