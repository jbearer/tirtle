#include <iostream>

#include "tirtle/stream.h"
#include "tirtle/stdio_stream.h"

using namespace tirtle;

stdio_ostream::stdio_ostream(std::ostream & inner_)
    : inner(inner_)
{}

void stdio_ostream::flush()
{
    inner.flush();
}

void stdio_ostream::write(const uint8_t *buf, streamsize_t n)
{
    inner.write(reinterpret_cast<const char*>(buf), n);
}

stdio_ostream::~stdio_ostream()
{
    flush();
}
