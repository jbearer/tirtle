#pragma once

#include <stdint.h>

typedef uint32_t coord_t;
typedef uint16_t length_t;
typedef uint16_t angle_t;

typedef struct {
    coord_t x;
    coord_t y;
} point_t;

typedef struct {
    const point_t         *points;
    length_t        length;
} path_t;
