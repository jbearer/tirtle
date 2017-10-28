#pragma once

#include <memory>
#include <string>
#include <vector>

#include "tirtle/path.h"

namespace tirtle {

    struct tirtle_client
    {
        /**
         * Send a set of paths to the Tirtle. The Tirtle will immediately begin tracing out the
         * given paths. This function must only be called once.
         */
        virtual void load_image(const std::vector<path_t> & paths) = 0;

        /**
         * Update the Tirtle with its current position and orientation.
         */
        virtual void set_position(point_t loc, angle_t angle) = 0;

        virtual ~tirtle_client() {}
    };

    /**
     * Connect to a Tirtle server via Bluetooth on the given device name.
     */
    std::unique_ptr<tirtle_client> connect_tirtle(const std::string & dev);

}
