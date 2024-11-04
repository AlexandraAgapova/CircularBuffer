cmake_minimum_required(VERSION 3.10)
project(circular_buffer)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(circular_buffer src/circular-buffer.cpp)

add_subdirectory(tests)
