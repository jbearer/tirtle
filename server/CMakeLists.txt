cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/avr-arduino-cmake/avr-arduino-toolchain.cmake)
project(tirtle)

file(GLOB SERVER_C_SRC src/*.c)
file(GLOB SERVER_CXX_SRC src/*.cpp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(PROGRAMMER arduino)

# Find Arduino libraries
set(ARDUINO_POSSIBLE_ROOT_DIRS
    "/usr/share/arduino")
find_path(ARDUINO_ROOT_DIR
    NAMES
    hardware/arduino/cores/arduino/Arduino.h
    PATHS ${ARDUINO_POSSIBLE_ROOT_DIRS})
if(NOT ARDUINO_ROOT_DIR)
    message(FATAL_ERROR "Could not find arduino library.")
endif()

set(ARDUINO_INCLUDE_DIRS
    "${ARDUINO_ROOT_DIR}/hardware/arduino/cores/arduino"
    "${ARDUINO_ROOT_DIR}/hardware/arduino/variants/standard"
    "${ARDUINO_ROOT_DIR}/libraries/SoftwareSerial")

include_directories(include ../common/include)
include_directories(${ARDUINO_INCLUDE_DIRS})

# Arduino core library
file(GLOB ARDUINO_CORE_C_SRC ${ARDUINO_ROOT_DIR}/hardware/arduino/cores/arduino/*.c)
file(GLOB ARDUINO_CORE_CXX_SRC ${ARDUINO_ROOT_DIR}/hardware/arduino/cores/arduino/*.cpp)
add_library(arduino-core ${ARDUINO_CORE_C_SRC} ${ARDUINO_CORE_CXX_SRC})

# SoftwareSerial library
add_library(software-serial ${ARDUINO_ROOT_DIR}/libraries/SoftwareSerial/SoftwareSerial.cpp)

add_executable_avr(server ${SERVER_C_SRC} ${SERVER_CXX_SRC})
target_link_libraries(server arduino-core software-serial)
