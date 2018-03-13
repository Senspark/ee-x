cmake_minimum_required(VERSION 3.1)

project(ee_x_notification)

include_directories(
  ../../
  ../../../third_party/nlohmann/include
)

set(LIB_SRC
  NotificationBridge.cpp
  NotificationBuilder.cpp
)

set(LIB_HEADERS
  NotificationBridge.hpp
  NotificationBuilder.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})