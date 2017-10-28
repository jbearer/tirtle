#include <iostream>
#include <vector>
#include <ctime>
#include "opencv2/opencv.hpp"
#include "opencv2/aruco.hpp"

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

void detectMarkers()
{
    cv::Mat inputImage = cv::imread("markers/test_markers.png");

    vector< int > markerIds;
    vector< vector<cv::Point2f> > markerCorners, rejectedCandidates;
    cv::aruco::DetectorParameters parameters;
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

    clock_t start = clock();
    cv::aruco::detectMarkers(inputImage, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);
    clock_t end = clock();

    for (auto id : markerIds) {
        cout << id << " ";
    } cout << endl;

    for (auto candidate : rejectedCandidates) {
        for (cv::Point2f pt : candidate) {
            cv::circle(inputImage, pt, 5, cv::Scalar(255, 0, 0), -1);
        }
    }

    cv::imwrite("output.png", inputImage);

    cout << "rejected candidates: " << rejectedCandidates.size() << endl;
    cout << float(end - start) / CLOCKS_PER_SEC << endl;
}


int main()
{
    detectMarkers();
    cout << "hello" << endl;
}
