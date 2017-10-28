project(tirtle C CXX)

cmake_minimum_required(VERSION 3.5)

# Built tree configuration
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# C++ configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPI_CXX -I/home/pi/opencv-3.3.1/include/ -I/usr/local/include/opencv -I/usr/local/include -L/usr/local/lib")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_BUILD_TYPE}" EQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DBUILD_RELEASE")
    set(LOG_LEVEL info)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DBUILD_DEBUG")
    set(LOG_LEVEL debug)
endif()

find_package(OpenCV COMPONENTS core imgproc imgcodecs highgui REQUIRED)

file(GLOB OPENCV_MODULE_HEADERS /home/pi/opencv-3.3.1/modules/*/include)

set(OpenCV_LIB_PATH /usr/local/lib/ )

# tirtle headers
include_directories(include)

file(GLOB_RECURSE CLIENT_SRC src/*.cpp)
add_library(tirtle-client SHARED ${CLIENT_SRC})
target_include_directories(tirtle-client PRIVATE ${OPENCV_MODULE_HEADERS})
target_link_libraries(tirtle-client bluetooth)
target_link_libraries(tirtle-client ${OpenCV_LIB_PATH}libopencv_aruco.so.3.1.0 opencv_core opencv_imgproc opencv_objdetect opencv_highgui opencv_shape opencv_stitching opencv_superres opencv_videostab opencv_calib3d opencv_features2d opencv_videoio opencv_imgcodecs opencv_video opencv_photo opencv_ml opencv_flann ncurses)

add_executable(test-client test/test_tirtle_client.cpp)
target_link_libraries(test-client tirtle-client)

add_executable(test-tirtle test/test_tirtle.cpp)
target_link_libraries(test-tirtle tirtle-client)

add_executable(test-e2e test/test_e2e.cpp)
target_link_libraries(test-e2e tirtle-client)
