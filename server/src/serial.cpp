#include <Arduino.h>

#include "tirtle/serial.h"

using namespace tirtle::serial;

void serial_ostream::flush()
{
    Serial.flush();
}

void serial_ostream::write(const uint8_t *buf, streamsize_t n)
{
    Serial.write(buf, n);
}

serial_ostream::~serial_ostream()
{
    flush();
}
