#pragma once

#include "tirtle/stream.h"

namespace tirtle {
    namespace serial {

        struct serial_ostream
            : text_ostream
        {
            void flush() override;
            void write(const uint8_t *buf, streamsize_t n) override;

            ~serial_ostream();
        };

        static serial_ostream out;
    }

}
