project(tirtle C CXX)

cmake_minimum_required(VERSION 3.5)

# Built tree configuration
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# C++ configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -DPI_CXX")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_BUILD_TYPE}" EQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DBUILD_RELEASE")
    set(LOG_LEVEL info)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DBUILD_DEBUG")
    set(LOG_LEVEL debug)
endif()

# tirtle headers
include_directories(include)

file(GLOB_RECURSE CLIENT_SRC src/*.cpp)
add_library(tirtle-client SHARED ${CLIENT_SRC})
target_link_libraries(tirtle-client bluetooth)

add_executable(test-client test/test_tirtle_client.cpp)
target_link_libraries(test-client tirtle-client)

add_executable(test-tirtle test/test_tirtle.cpp)
target_link_libraries(test-tirtle tirtle-client)
