cmake_minimum_required(VERSION 3.5)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/avr-arduino-cmake/avr-arduino-toolchain.cmake)
project(tirtle)

file(GLOB TIRTLE_COMMON_SRC ../common/src/*.cpp)
file(GLOB SERVER_SRC src/*.cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DARDUINO -Os")
set(CXX_WERROR_FLAGS "-Wall -Wextra -Werror")

set(PROGRAMMER arduino)
set(SERIAL_DEV /dev/ttyACM0)

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
target_link_libraries(software-serial arduino-core)

set(ARDUINO_LIBS arduino-core software-serial)

# tirtle server library
add_library(tirtle-server STATIC ${SERVER_SRC} ${TIRTLE_COMMON_SRC})
set_target_properties(tirtle-server PROPERTIES COMPILE_FLAGS ${CXX_WERROR_FLAGS})
target_link_libraries(tirtle-server ${ARDUINO_LIBS})

# main server executable
add_executable_avr(server main.cpp)
set_target_properties(server PROPERTIES COMPILE_FLAGS ${CXX_WERROR_FLAGS})
target_link_libraries(server tirtle-server)
target_link_libraries(server ${ARDUINO_LIBS})

# test RPC server
add_executable_avr(test-echo test/echo.cpp)
set_target_properties(test-echo PROPERTIES COMPILE_FLAGS ${CXX_WERROR_FLAGS})
target_link_libraries(test-echo tirtle-server)
target_link_libraries(test-echo ${ARDUINO_LIBS})
