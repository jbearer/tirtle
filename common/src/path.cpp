#include "tirtle/path.h"
#include "tirtle/stream.h"

using tirtle::binary_istream;
using tirtle::binary_ostream;
using tirtle::text_ostream;

tirtle::point::point()
    : point(0, 0)
{}

tirtle::point::point(coord_t x_, coord_t y_)
    : x(x_)
    , y(y_)
{}

bool tirtle::point::operator==(const tirtle::point & that) const
{
    return x == that.x && y == that.y;
}

bool tirtle::point::operator!=(const tirtle::point & that) const
{
    return !(*this == that);
}

tirtle::point tirtle::point::operator+(const tirtle::point & that) const
{
    return point(x + that.x, y + that.y);
}

binary_istream & tirtle::operator>>(binary_istream & in, tirtle::point & p)
{
    return in >> p.x >> p.y;
}

binary_ostream & tirtle::operator<<(binary_ostream & out, const tirtle::point & p)
{
    return out << p.x << p.y;
}

text_ostream & tirtle::operator<<(text_ostream & out, const tirtle::point & loc)
{
    return out << "(" << loc.x << ", " << loc.y << ")";
}
