cmake_minimum_required(VERSION 3.5)
project(travelling-salesman-problem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES TSP.cpp)
add_executable(travelling-salesman-problem ${SOURCE_FILES})
