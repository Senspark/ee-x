cmake_minimum_required(VERSION 3.1)

project(jansson)
add_library(${PROJECT_NAME} "")

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/sourcelist.cmake)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)