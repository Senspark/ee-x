cmake_minimum_required(VERSION 3.1)

project(ee_x_core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(
  ../../
  internal
  ../../../third_party/nlohmann/include
)

set(LIB_SRC
   Logger.cpp
   LogLevel.cpp
   Metrics.cpp
   Utils.cpp
   MessageBridge.cpp
)

set(LIB_HEADERS
   Logger.hpp
   LogLevel.hpp
   Metrics.hpp
   Utils.hpp
   MessageBridge.hpp
   internal/ScopeGuard.hpp
   internal/SpinLock.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
  ./
)
