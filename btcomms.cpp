#include "btcomms.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define BT_DEBUG

BTComms::BTComms(const char *localName)
    : m_localName(localName)
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
        Serial.write(m_BT.read());
    }
}