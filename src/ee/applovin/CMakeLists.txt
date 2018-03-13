cmake_minimum_required(VERSION 3.1)

project(ee_x_applovin)

include_directories(
  ../..
  internal
  ../../../third_party/nlohmann/include
)

set(LIB_SRC
  AppLovinBridge.cpp
  internal/AppLovinRewardedVideo.cpp
)

set(LIB_HEADERS
  AppLovinBridge.hpp
  internal/AppLovinRewardedVideo.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})