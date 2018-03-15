cmake_minimum_required(VERSION 3.1)

# project(ee_x_applovin)

include_directories(
  src/ee/applovin
  src/ee/applovin/internal
  third_party/nlohmann/include
)

set(EE_X_APPLOVIN_SRC
  src/ee/applovin/AppLovinBridge.cpp
  src/ee/applovin/internal/AppLovinRewardedVideo.cpp
)

set(EE_X_APPLOVIN_HEADERS
  src/ee/applovin/AppLovinBridge.hpp
  src/ee/applovin/internal/AppLovinRewardedVideo.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})