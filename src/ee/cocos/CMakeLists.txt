cmake_minimum_required(VERSION 3.1)

project(ee_x_cocos)
add_library(${PROJECT_NAME} "")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wchar-subscripts -Wcomment -Wformat=2 -Wnonnull -Winit-self")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmain -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wreturn-type -Waddress -Wmissing-declarations -Wmissing-field-initializers")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-function -Wunused-label -Wuninitialized")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/sourcelist.cmake)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../../src
)

target_link_libraries(${PROJECT_NAME} cocos2d)