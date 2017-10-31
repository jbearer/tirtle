#pragma once

#include "tirtle/path.h"

namespace tirtle {

    struct robot_impl;

    struct robot
    {
        robot();
        robot(const robot &) = delete;

        robot & operator=(const robot &) = delete;

        void set_position(const point & loc, angle_t angle);
        void load_image(const image & img);
        void step();

        ~robot();

    private:
        robot_impl *impl;
    };

}
