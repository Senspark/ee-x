cmake_minimum_required(VERSION 3.1)

project(ee_x_crashlytics)

include_directories(
  ../../
  ../../../third_party/nlohmann/include
)

set(LIB_SRC
  CrashlyticsBridge.cpp
)

set(LIB_HEADERS
  CrashlyticsBridge.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

target_link_libraries(${PROJECT_NAME}
  ee_x_core
)