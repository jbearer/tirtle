#include <atomic>
#include <iostream>
#include <vector>
#include <ctime>
#include <tuple>
#include <memory>
#include "math.h"
#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"

#include "tirtle/path.h"
#include "tirtle/tirtle_client.h"
#include "tirtle/tracker.h"

using namespace std;

void drawMarkers()
{
    for (int i = 0; i < 5; ++i) {
        cv::Mat markerImage;
        cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
        cv::aruco::drawMarker(dictionary, i, 200, markerImage, 1);
        imwrite("markers/marker" + to_string(i) + ".png", markerImage);
    }
}

vector<cv::Point2f> getCorners(int index, vector< int > markerIds, vector< vector<cv::Point2f> > markerCorners)
{
    for (int i = 0; i < markerIds.size(); ++i) {
        if (markerIds[i] == index) {
            return markerCorners[i];
        }
    }
}

std::tuple< vector<vector<cv::Point2f>>, vector<int>>  detectMarkers(cv::Mat& inputImage)
{
    vector< int > markerIds;
    vector< vector<cv::Point2f> > markerCorners, rejectedCandidates;
    cv::aruco::DetectorParameters parameters;
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    clock_t start = clock();
    cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);
    clock_t end = clock();

    cout << "rejected candidates: " << rejectedCandidates.size() << endl;
    cout << float(end - start) / CLOCKS_PER_SEC << endl;

    int num_markers = 5;

    return std::make_tuple(markerCorners, markerIds);

}

vector<cv::Point2f> getOuterCorners(vector<vector<cv::Point2f>> corners, vector<int> ids)
{

    // top left of corner 1
    auto c1 = getCorners(1, ids, corners)[0];
    auto c2 = getCorners(2, ids, corners)[1];
    auto c3 = getCorners(3, ids, corners)[2];
    auto c4 = getCorners(4, ids, corners)[3];

    vector<cv::Point2f> outer_corners{c1, c2, c3, c4};
    return outer_corners;
}

float getAngle(cv::Mat turtle_corners)
{
    cv::Point2f a = turtle_corners.at<cv::Point2f>(3);
    cv::Point2f d = turtle_corners.at<cv::Point2f>(2);

    float delta_y = d.y - a.y;
    float delta_x = d.x - a.x;
    float radians = atan2(delta_y, delta_x);
    float degrees = radians / M_PI * 180.0;

    if (degrees < 0) {
        return degrees + 360.0;
    }
    else {
        return degrees;
    }
}

tuple<cv::Point2f, float> findPoint(vector<cv::Point2f> outer_corners, vector<cv::Point2f> robot_corners)
{

    int width = 200;
    int height = 150;

    cv::Point2f bottom_left = cv::Point2f(0, 0);
    cv::Point2f top_left = cv::Point2f(0, height);
    cv::Point2f top_right = cv::Point2f(width, height);
    cv::Point2f bottom_right = cv::Point2f(width, 0);

    vector<cv::Point2f> turtle_corners{top_left, top_right, bottom_right, bottom_left};
    cv::Mat transform = cv::getPerspectiveTransform(outer_corners, turtle_corners);

    cv::Mat input{robot_corners};
    cv::Mat output;
    //vector<Point2f> inputPoint{Point2f(0,0)};
    //vector<Point2f> outputPoint;
    perspectiveTransform(input, output, transform);

    float angle = getAngle(output);
    return make_tuple(output.at<cv::Point2f>(0), angle); // TODO: Make this the center
}

unique_ptr<tuple<cv::Point2f, float>> turtle_pos(cv::Mat& inputImage)
{
    auto corner_info = detectMarkers(inputImage);
    auto corners = get<0>(corner_info);
    auto ids = get<1>(corner_info);

    if (corners.size() != 5){
        return nullptr;
    }

    vector<cv::Point2f> outer_corners = getOuterCorners(corners, ids);

    auto point_angle = findPoint(outer_corners, robot_corners);
    cv::imwrite("output.png", inputImage);
    return make_unique<tuple<cv::Point2f, float>>(point_angle);
}

static void track(point_t & loc, angle_t & heading)
{
    // TODO grab one frame and analyze it
}

static void loop_track(tirtle_client & client, std::atomic<bool> & halt)
{
    while (!halt.load()) {
        point_t loc;
        angle_t heading;
        track(loc, heading);
        client.set_position(loc, heading);
    }
}

tirtle::tracker::tracker(tirtle_client & client)
    : halt(false)
    , loop(std::async(std::launch::async, loop_track, client, halt))
{}

tirtle::tracker::~tracker()
{
    halt.store(true);
    loop.get(); // wait for loop to stop
}

static void track(point_t & loc, angle_t & heading)
{
    // TODO grab one frame and analyze it
}

static void loop_track(tirtle_client & client, std::atomic<bool> & halt)
{
    while (!halt.load()) {
        point_t loc;
        angle_t heading;
        track(loc, heading);
        client.set_position(loc, heading);
    }
}

tirtle::tracker::tracker(tirtle_client & client)
    : halt(false)
    , loop(std::async(std::launch::async, loop_track, client, halt))
{}

tirtle::tracker::~tracker()
{
    halt.store(true);
    loop.get(); // wait for loop to stop
}
