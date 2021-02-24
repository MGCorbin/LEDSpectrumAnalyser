
#pragma once

#include "leds.h"
#include "BluetoothSerial.h"

class BTComms
{
public:
    BTComms(const char *name, Leds &leds);

    void init();
    void write();
    void read();

private:
    const char *m_localName;
    BluetoothSerial m_BT;
    Leds &m_leds;           // hold a reference to leds so we can change mode etc via bt
};