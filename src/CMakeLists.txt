# Configurations for Cmake
cmake_minimum_required (VERSION 3.2.2)
project (skywalker_prototype)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH})
set(LIBRARY_OUTPUT_PATH ../bin)
set(EXECUTABLE_OUTPUT_PATH ../bin)

# Building the binaries
add_executable(skywalker_prototype
                        launch.cpp
                        spibus.h spibus.cpp
                        bus.h
                        icdevice.h icdevice.cpp
                        sensor.h
                        accelerometer.h
                        thermometer.h
                        gyroscope.h
                        magnetometer.h
                        st_sensor.h st_sensor.cpp
                        st_accel_dsh.h st_accel_dsh.cpp
                        #st_magno_mdl.h st_magno_mdl.cpp
                        #st_gyro_20h.h st_gyro_20h.cpp
                        error.h
                        types.h)
