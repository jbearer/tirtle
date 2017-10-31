#pragma once

#include <iostream>

#include "tirtle/stream.h"

namespace tirtle {

    // Wrap an std::ostream
    struct stdio_ostream
        : text_ostream
    {

        stdio_ostream(std::ostream &);

        void flush() override;
        void write(const uint8_t *buf, streamsize_t n) override;

        ~stdio_ostream();

    private:
        std::ostream & inner;
    };

    static stdio_ostream cout(std::cout);
    static stdio_ostream cerr(std::cout);

}
