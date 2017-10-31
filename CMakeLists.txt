cmake_minimum_required(VERSION 3.5)

project(tirtle)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(TIRTLE_COMMON_INCLUDE common/include)
file(GLOB TIRTLE_COMMON_SRC common/src/*.cpp)

ExternalProject_Add(client
    SOURCE_DIR client
    BINARY_DIR client/build
    INSTALL_COMMAND :)
ExternalProject_Add(server
    SOURCE_DIR server
    BINARY_DIR server/build
    INSTALL_COMMAND :)
