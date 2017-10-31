#pragma once

#ifndef ARDUINO
#include <string>
#endif

#include <inttypes.h>
#include <stdint.h>

namespace tirtle {

    using streamsize_t = uint32_t;

    struct istream
    {
        virtual streamsize_t available() const = 0;
        virtual void read(uint8_t *buf, streamsize_t n) = 0;
        virtual ~istream() {}
    };

    struct binary_istream : istream {};

    binary_istream & operator>>(binary_istream &, uint8_t &);
    binary_istream & operator>>(binary_istream &, uint16_t &);
    binary_istream & operator>>(binary_istream &, uint32_t &);

    struct ostream
    {
        virtual void flush() = 0;
        virtual void write(const uint8_t *buf, streamsize_t n) = 0;
        virtual ~ostream() {}
    };

    struct binary_ostream : ostream {};
    struct text_ostream : ostream {};

    binary_ostream & operator<<(binary_ostream &, uint8_t);
    binary_ostream & operator<<(binary_ostream &, uint16_t);
    binary_ostream & operator<<(binary_ostream &, uint32_t);

    text_ostream & operator<<(text_ostream &, uint8_t);
    text_ostream & operator<<(text_ostream &, uint16_t);
    text_ostream & operator<<(text_ostream &, uint32_t);
    text_ostream & operator<<(text_ostream &, int8_t);
    text_ostream & operator<<(text_ostream &, int16_t);
    text_ostream & operator<<(text_ostream &, int32_t);
#ifdef PRId64
    text_ostream & operator<<(text_ostream &, int64_t);
#endif
    text_ostream & operator<<(text_ostream &, const char *);
#ifndef ARDUINO
    text_ostream & operator<<(text_ostream &, const std::string &);
#endif

    template<class stream>
    stream & endl(stream & out)
    {
        out << "\r\n";
        out.flush();
        return out;
    }

    template<class stream>
    stream & operator<<(stream & out, stream &(*manip)(stream &))
    {
        return manip(out);
    }

}
