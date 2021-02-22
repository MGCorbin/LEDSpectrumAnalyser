#include <memory>

#include "BluetoothSerial.h"

class BTComms
{
public:
    BTComms(const char *name);

    void write();
    void read();

private:
    const char *m_localName;
    static BluetoothSerial m_BT;
};