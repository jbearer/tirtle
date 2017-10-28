#pragma once

#ifdef PI_CXX
#include <iostream>
#include <vector>
#endif

#include <stdint.h>

typedef uint32_t coord_t;
typedef uint16_t length_t;
typedef uint8_t angle_t;

// Point in the first qudrant of 2D cartesian space
typedef struct {
    coord_t x;
    coord_t y;
} point_t;

// Path through the first quadrant of 2D cartesian space
typedef struct {
    length_t        length;   // Number of points in the path
    point_t         *points;  // Points to reach (in order) to trace out the path
} path_t;

#ifdef PI_CXX
// Helper functions only defined for the Raspberry PI client
// The Arduino can't spare the memory, plus it doesn't even support, eg, iostream

std::ostream & operator<<(std::ostream & out, const point_t & loc)
{
    return out << "(" << loc.x << ", " << loc.y << ")";
}

inline point_t make_point(coord_t x, coord_t y)
{
    point_t p;
    p.x = x;
    p.y = y;
    return p;
}

inline path_t make_path(const std::vector<point_t> & points)
{
    path_t p;
    p.length = points.size();
    p.points = const_cast<point_t*>(points.data());
    return p;
}

#endif
