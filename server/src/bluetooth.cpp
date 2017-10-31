#include <SoftwareSerial.h>

#include "tirtle/bluetooth.h"

using namespace tirtle;

bluetooth_stream::bluetooth_stream(int rx, int tx)
    : bluetooth_stream(rx, tx, 9600)
{}

bluetooth_stream::bluetooth_stream(int rx, int tx, unsigned baud)
    : bt(rx, tx)
{
    bt.begin(baud);
}

streamsize_t bluetooth_stream::available() const
{
    return const_cast<SoftwareSerial&>(bt).available();
}

void bluetooth_stream::read(uint8_t *buf, streamsize_t n)
{
    while (available() < n) {}
    bt.readBytes(reinterpret_cast<char *>(buf), n);
}

void bluetooth_stream::write(const uint8_t *buf, streamsize_t n)
{
    bt.write(buf, n);
}

void bluetooth_stream::flush()
{
    return bt.flush();
}

bluetooth_stream::~bluetooth_stream()
{
    flush();
}
