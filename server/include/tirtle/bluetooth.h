#pragma once

#include <SoftwareSerial.h>

#include "tirtle/stream.h"

namespace tirtle {

    struct bluetooth_stream
        : binary_istream
        , binary_ostream
    {
        bluetooth_stream(int rx, int tx);
        bluetooth_stream(int rx, int tx, unsigned baud);

        streamsize_t available() const override;
        void read(uint8_t* buf, streamsize_t n) override;
        void write(const uint8_t* buf, streamsize_t n) override;
        void flush() override;

        ~bluetooth_stream();

    private:
        SoftwareSerial bt;
    };

}
