cmake_minimum_required(VERSION 3.1)

project(soomla_cocos2dx_store)
add_library(${PROJECT_NAME} "")

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/sourcelist.cmake)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${PROJECT_NAME} soomla_cocos2dx_core)