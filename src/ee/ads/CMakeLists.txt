cmake_minimum_required(VERSION 3.1)

project(ee_x_ads)

include_directories(
  ../../
  ../../../third_party/nlohmann/include
  internal
)

set(LIB_SRC
  AdViewInterface.cpp
  InterstitialAdInterface.cpp
  MultiAdView.cpp
  MultiInterstitialAd.cpp
  MultiRewardedVideo.cpp
  RewardedVideoInterface.cpp
  internal/AdViewBridgeHelper.cpp
  internal/AdViewHelper.cpp
)

set(LIB_HEADERS
  AdViewInterface.hpp
  InterstitialAdInterface.hpp
  MultiAdView.hpp
  MultiInterstitialAd.hpp
  MultiRewardedVideo.hpp
  NullAdView.hpp
  NullInterstitialAd.hpp
  NullRewardedVideo.hpp
  RewardedVideoInterface.hpp
  internal/AdViewBridgeHelper.hpp
  internal/AdViewHelper.hpp
  internal/MediationManager.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
  ../../
)
