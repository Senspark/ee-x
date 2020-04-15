cmake_minimum_required(VERSION 3.1)

project(ee_x_cocos_coroutine)
add_library(${PROJECT_NAME} "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fcoroutines-ts")

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/sourcelist.cmake)

target_link_libraries(${PROJECT_NAME} cocos2d ee_x_coroutine)