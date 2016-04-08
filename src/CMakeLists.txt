# Configurations for Cmake
cmake_minimum_required (VERSION 3.2.2)
project (spibus)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH})
set(LIBRARY_OUTPUT_PATH ../bin)
set(EXECUTABLE_OUTPUT_PATH ../bin)

# cross comipile stuff for beaglebone black
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabi-g++)
SET(CMAKE_FIND_ROOT_PATH /usr/arm-linux-gnueabi)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv7-a -mthumb-interwork -mfloat-abi=softfp -mfpu=neon -mtune=cortex-a8 --static")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# Building the binaries
add_executable(spibus   launch.cpp
                        spibus.h spibus.cpp
                        bus.h
                        icdevice.h icdevice.cpp
                        sensor.h
                        accelerometer.h
                        thermometer.h
                        gyroscope.h
                        magnetometer.h
                        st_accel_dsh.h st_accel_dsh.cpp
                        error.h)
