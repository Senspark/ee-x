cmake_minimum_required(VERSION 3.1)

# project(ee_x_iron_source)

include_directories(
  third_party/nlohmann/include
  src/ee/iron_source/internal
  src/ee/iron_source
)

set(EE_X_IRON_SOURCE_SRC
  src/ee/iron_source/IronSourceBridge.cpp
  src/ee/iron_source/internal/IronSourceInterstitialAd.cpp
  src/ee/iron_source/internal/IronSourceRewardedVideo.cpp
)

set(EE_X_IRON_SOURCE_HEADERS
  src/ee/iron_source/IronSourceBridge.hpp
  src/ee/iron_source/internal/IronSourceInterstitialAd.hpp
  src/ee/iron_source/internal/IronSourceRewardedVideo.hpp
)
