#pragma once

#include <memory>
#include <string>
#include <vector>

#include "tirtle/path.h"

namespace tirtle {

    struct tirtle_client_impl;

    struct tirtle_client
    {
        tirtle_client(const std::string & dev);

        void load_image(const std::vector<path_t> & paths);
        void set_position(point_t loc, angle_t angle);

        ~tirtle_client();

    private:
        tirtle_client_impl *impl;
    };

}
