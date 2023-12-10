cmake_minimum_required(VERSION 3.1)

project(advent2023)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(day1_1 day1_1/main.cpp)
add_executable(day1_2 day1_2/main.cpp)
add_executable(day2_1 day2_1/main.cpp)
add_executable(day2_2 day2_2/main.cpp)