cmake_minimum_required(VERSION 3.17)
project(ABCHW4)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(ABCHW4 main.cpp)