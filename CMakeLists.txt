cmake_minimum_required(VERSION 3.22)
project(serial_sensors)
find_package(Boost REQUIRED)

add_executable(Main)
target_include_directories(Main PUBLIC ${PROJECT_SOURCE_DIR}/include)
set(programs
    src/main.cpp
    src/Gyro.cpp
    src/GNSS.cpp
    src/Solution2Motors.cpp)
target_sources(Main PRIVATE ${programs})
