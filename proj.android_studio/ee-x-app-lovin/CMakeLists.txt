cmake_minimum_required(VERSION 3.1)

include(${CMAKE_CURRENT_LIST_DIR}/../../src/ee/app_lovin/CMakeLists.txt)

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++2a -fcoroutines-ts)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../src
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../third_party/nlohmann/include
)