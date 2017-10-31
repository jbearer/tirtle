#ifndef ARDUINO
#include <string>
#endif

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "tirtle/stream.h"

using tirtle::binary_istream;
using tirtle::binary_ostream;
using tirtle::text_ostream;

#define RPC_ISTREAM_PRIMITIVE_READ(T) \
binary_istream & tirtle::operator>>(binary_istream & in, T & prim) \
{ \
    in.read(reinterpret_cast<uint8_t*>(&prim), sizeof(T)); \
    return in; \
}

RPC_ISTREAM_PRIMITIVE_READ(uint8_t);
RPC_ISTREAM_PRIMITIVE_READ(uint16_t);
RPC_ISTREAM_PRIMITIVE_READ(uint32_t);

#define RPC_OSTREAM_PRIMITIVE_WRITE(T) \
binary_ostream & tirtle::operator<<(binary_ostream & out, T prim) \
{ \
    out.write(reinterpret_cast<const uint8_t*>(&prim), sizeof(T)); \
    return out; \
}

RPC_OSTREAM_PRIMITIVE_WRITE(uint8_t);
RPC_OSTREAM_PRIMITIVE_WRITE(uint16_t);
RPC_OSTREAM_PRIMITIVE_WRITE(uint32_t);

text_ostream & tirtle::operator<<(text_ostream & out, int8_t prim)
{
    char str[5]; // 3 decimal digits + sign + null character
    snprintf(str, 5, "%" PRId8, prim);
    return out << str;
}

text_ostream & tirtle::operator<<(text_ostream & out, int16_t prim)
{
    char str[7]; // 5 decimal digits + sign + null character
    snprintf(str, 7, "%" PRId16, prim);
    return out << str;
}

text_ostream & tirtle::operator<<(text_ostream & out, int32_t prim)
{
    char str[12]; // 10 decimal digits + sign + null character
    snprintf(str, 12, "%" PRId32, prim);
    return out << str;
}

#ifdef PRId64
text_ostream & tirtle::operator<<(text_ostream & out, int64_t prim)
{
    char str[21]; // 19 decimal digits + sign + null character
    snprintf(str, 21, "%" PRId64, prim);
    return out << str;
}
#endif

text_ostream & tirtle::operator<<(text_ostream & out, uint8_t prim)
{
    char str[4]; // 3 decimal digits + null character
    snprintf(str, 4, "%" PRIu8, prim);
    return out << str;
}

text_ostream & tirtle::operator<<(text_ostream & out, uint16_t prim)
{
    char str[6]; // 5 decimal digits + null character
    snprintf(str, 6, "%" PRIu16, prim);
    return out << str;
}

text_ostream & tirtle::operator<<(text_ostream & out, uint32_t prim)
{
    char str[11]; // 10 decimal digits + null character
    snprintf(str, 11, "%" PRIu32, prim);
    return out << str;
}

text_ostream & tirtle::operator<<(text_ostream & out, const char *str)
{
    out.write(reinterpret_cast<const uint8_t*>(str), strlen(str));
    return out;
}

#ifndef ARDUINO
text_ostream & tirtle::operator<<(text_ostream & out, const std::string & str)
{
    return out << str.c_str();
}
#endif
