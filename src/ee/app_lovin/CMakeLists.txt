cmake_minimum_required(VERSION 3.1)

# project(ee_x_app_lovin)

include_directories(
  src/ee/app_lovin
  src/ee/app_lovin/internal
  third_party/nlohmann/include
)

set(EE_X_APPLOVIN_SRC
  src/ee/app_lovin/AppLovinBridge.cpp
  src/ee/app_lovin/internal/AppLovinRewardedVideo.cpp
)

set(EE_X_APPLOVIN_HEADERS
  src/ee/app_lovin/AppLovinBridge.hpp
  src/ee/app_lovin/internal/AppLovinRewardedVideo.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})