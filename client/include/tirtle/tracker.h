#pragma once

#include <atomic>
#include <future>
#include "tirtle/tirtle_client.h"

namespace tirtle {

    struct tracker
    {
        tracker(tirtle_client &);
        ~tracker();

    private:
        std::atomic<bool>   halt;
        std::future<void>   loop;
    };

}
