#pragma once

#include "tirtle/path.h"
#include "tirtle/tirtle_client.h"

namespace tirtle {

    const point start_loc(50, 50);
    const point canvas_size(300, 200);

    struct tirtle_impl;

    struct tirtle
    {
        tirtle();
        ~tirtle();

        point get_position() const;
        angle_t get_orientation() const;

        tirtle & forward(length_t);
        tirtle & backward(length_t);
        tirtle & right(angle_t);
        tirtle & left(angle_t);

        tirtle & move_to(const point &);
        tirtle & turn_to(angle_t);

        tirtle & pen_up();
        tirtle & pen_down();

        void draw(tirtle_client &);

    private:
        tirtle_impl *impl;
    };

}