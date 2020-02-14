cmake_minimum_required(VERSION 3.1)

project(ee_x_coroutine)
add_library(${PROJECT_NAME} "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines-ts")