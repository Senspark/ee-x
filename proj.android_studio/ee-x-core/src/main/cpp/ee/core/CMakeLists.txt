cmake_minimum_required(VERSION 3.1)

project(ee_x_core)
add_library(${PROJECT_NAME} STATIC "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines-ts")

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/sourcelist.cmake)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../
)