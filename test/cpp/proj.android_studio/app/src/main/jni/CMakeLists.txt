cmake_minimum_required(VERSION 3.1)

project(ee_x_test)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../..)
set(COCOS2DX_ROOT_PATH ${PROJECT_ROOT_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules)

include(CocosBuildSet)

set(GAME_RES_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/Resources")

add_library(${PROJECT_NAME} SHARED "")

execute_process(COMMAND sh generate.sh WORKING_DIRECTORY ${PROJECT_ROOT_DIR}/Classes)
include(${PROJECT_ROOT_DIR}/Classes/sourcelist.cmake)
include(${PROJECT_ROOT_DIR}/Classes/includelist.cmake)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_ROOT_DIR}/Classes
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include
)

add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
add_subdirectory(${PROJECT_ROOT_DIR}/../.. ${ENGINE_BINARY_PATH}/ee-x)

target_link_libraries(${PROJECT_NAME}
    -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive
    cocos2d
    soomla_store
    ee_x
)

setup_cocos_app_config(${PROJECT_NAME})