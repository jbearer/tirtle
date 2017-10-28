#pragma once

#ifdef PI_CXX
#include <iostream>
#include <vector>
#endif

#include <stdint.h>

typedef uint32_t coord_t;
typedef uint16_t length_t;
<<<<<<< HEAD
typedef uint8_t angle_t;
=======
typedef uint16_t angle_t;
>>>>>>> 5bad83f39634c8a5b6a28905b74f380b03542ffb

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

<<<<<<< HEAD
std::ostream & operator<<(std::ostream & out, const point_t & loc)
=======
inline std::ostream & operator<<(std::ostream & out, const point_t & loc)
>>>>>>> 5bad83f39634c8a5b6a28905b74f380b03542ffb
{
    return out << "(" << loc.x << ", " << loc.y << ")";
}

<<<<<<< HEAD
=======
inline bool operator==(const point_t & p1, const point_t & p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator!=(const point_t & p1, const point_t & p2)
{
    return !(p1 == p2);
}

inline bool operator==(const path_t & p1, const path_t & p2)
{
    if (p1.length != p2.length) return false;

    for (length_t i = 0; i < p1.length; ++i) {
        if (p1.points[i] != p2.points[i]) return false;
    }

    return true;
}

inline bool operator!=(const path_t & p1, const path_t & p2)
{
    return !(p1 == p2);
}

>>>>>>> 5bad83f39634c8a5b6a28905b74f380b03542ffb
inline point_t make_point(coord_t x, coord_t y)
{
    point_t p;
    p.x = x;
    p.y = y;
    return p;
}

/**
 * Construct a path using the given list of points.
 * N.B. The resulting path will share memory with the input vector. Thus, the lifetime of the vector
 *      must exceed the lifetime of the path.
 * N.B. Uses const_cast to convert points.data() to a non-const point_t*. The points of the
 *      resulting path must not be modified.
 */
inline path_t make_path(const std::vector<point_t> & points)
{
    path_t p;
    p.length = points.size();
    p.points = const_cast<point_t*>(points.data());
    return p;
}

#endif
