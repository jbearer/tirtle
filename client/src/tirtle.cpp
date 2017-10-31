#include <cmath>
#include <vector>

#include <string.h>

#include "tirtle/logging.h"
#include "tirtle/path.h"
#include "tirtle/tirtle.h"
#include "tirtle/tirtle_client.h"

static double deg2rad(tirtle::coord_t deg)
{
    return (deg*M_PI) / 180.0;
}

namespace tirtle {

    struct tirtle_impl
    {
        tirtle_impl()
            : loc(start_loc)
            , heading(0)
            , drawing(true)
        {}

        point get_position() const
        {
            return loc;
        }

        angle_t get_orientation() const
        {
            return heading;
        }

        void move_to(point dest)
        {
            log::debug("moving to point ", dest, " (facing ", (int)heading, ")");

            if (dest.x > canvas_size.x || dest.y > canvas_size.y) {
                log::error("point ", dest, " is not on canvas of size ", canvas_size);
                return;
            }

            if (drawing) {
                current_path.push_back(loc);
            }
            loc = dest;
        }

        void turn_to(angle_t dest)
        {
            log::debug("rotating to heading ", (int)dest);
            heading = dest;
        }

        void pen_up()
        {
            if (drawing) {
                finish_path();
            }
            drawing = false;
        }

        void pen_down()
        {
            drawing = true;
        }

        void draw(tirtle_client & client)
        {
            if (drawing) finish_path();
            client.load_image(image(paths));
        }

    private:

        void finish_path()
        {
            log::debug("finishing path at point ", loc);
            current_path.push_back(loc);
            paths.emplace_back(std::move(current_path));
            current_path.clear();
        }

        point                 loc;
        angle_t               heading;
        bool                  drawing;
        std::vector<point>    current_path;
        std::vector<path>     paths;
    };

    tirtle::tirtle()
        : impl(new tirtle_impl)
    {}

    tirtle::~tirtle()
    {
        delete impl;
    }

    void tirtle::forward(length_t delta)
    {
        point dest = get_position();
        angle_t heading = get_orientation();

        double delx = delta * std::cos(deg2rad(heading));
        if (delx < 0 && (coord_t)(-delx) > dest.x) {
            log::fatal("moving to negative x-coordinate ", (long)dest.x + (long)delx);
        } else {
            dest.x += (coord_t)std::round(delx);
        }

        double dely = delta * std::sin(deg2rad(heading));
        if (dely < 0 && (coord_t)(-dely) > dest.y) {
            log::fatal("moving to negative y-coordinate ", (long)dest.y + (long)dely);
        } else {
            dest.y += (coord_t)std::round(dely);
        }

        move_to(dest);
    }

    void tirtle::backward(length_t delta)
    {
        right(180);
        forward(delta);
        right(180);
    }

    void tirtle::right(angle_t delta)
    {
        left(360 - (delta % 360));
    }

    void tirtle::left(angle_t delta)
    {
        delta = delta % 360;
        turn_to((delta + get_orientation()) % 360);
    }

    void tirtle::move_to(const point & loc)
    {
        impl->move_to(loc);
    }

    void tirtle::turn_to(angle_t angle)
    {
        impl->turn_to(angle);
    }

    void tirtle::draw(tirtle_client & client)
    {
        impl->draw(client);
    }

    void tirtle::pen_up()
    {
        impl->pen_up();
    }

    void tirtle::pen_down()
    {
        impl->pen_down();
    }

    point tirtle::get_position() const
    {
        return impl->get_position();
    }

    angle_t tirtle::get_orientation() const
    {
        return impl->get_orientation();
    }

}
