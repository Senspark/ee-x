cmake_minimum_required(VERSION 3.1)

project(ee_x)
add_library(${PROJECT_NAME} "")
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++2a -fcoroutines-ts)

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/sourcelist.cmake)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/..
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../../third_party/nlohmann/include
)

target_link_libraries(${PROJECT_NAME} cocos2d)
