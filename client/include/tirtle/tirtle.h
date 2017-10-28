#pragma once

#include "tirtle/path.h"
#include "tirtle/tirtle_client.h"

namespace tirtle {

    struct tirtle_impl;

    struct tirtle
    {
        tirtle();
        ~tirtle();

        point_t get_position() const;
        angle_t get_orientation() const;

        void forward(length_t);
        void backward(length_t);
        void right(angle_t);
        void left(angle_t);

        void move_to(point_t);
        void turn_to(angle_t);

        void pen_up();
        void pen_down();

        void draw(tirtle_client &);

    private:
        tirtle_impl *impl;
    };

}