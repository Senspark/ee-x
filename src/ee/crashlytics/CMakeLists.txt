cmake_minimum_required(VERSION 3.1)

project(ee_x_crashlytics)
add_library(${PROJECT_NAME} "")

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/sourcelist.cmake)

target_link_libraries(${PROJECT_NAME} ee_x_core)
