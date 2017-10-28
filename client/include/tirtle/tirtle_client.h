#pragma once

#include <memory>
#include <string>
#include <vector>

#include "tirtle/path.h"

namespace tirtle {

    struct tirtle_client_impl;

    struct tirtle_client
    {
        /**
         * Connect to a Tirtle server via Bluetooth on the given device name.
         */
        tirtle_client(const std::string & dev);

        /**
         * Send a set of paths to the Tirtle. The Tirtle will immediately begin tracing out the
         * given paths. This function must only be called once.
         */
        void load_image(const std::vector<path_t> & paths);

        /**
         * Update the Tirtle with its current position and orientation.
         */
        void set_position(point_t loc, angle_t angle);

        ~tirtle_client();

    private:
        tirtle_client_impl *impl;
    };

}
