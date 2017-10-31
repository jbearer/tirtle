#pragma once

#include <atomic>
#include <future>
#include "tirtle/tirtle_client.h"
#include "opencv2/opencv.hpp"

namespace tirtle {

    struct tracker
    {
        tracker(tirtle_client &);
        ~tracker();

    private:
        void track();
        int loop_track();

        tirtle_client &     client;
        std::atomic<bool>   halt;
        std::future<int>    loop;
        cv::VideoCapture    v;
    };

}
