cmake_minimum_required(VERSION 3.1)

project(ee_x_facebook_ads)

include_directories(
  ../../
  internal
  ../../../third_party/nlohmann/include
)

set(LIB_SRC
   FacebookAdsBridge.cpp
   FacebookNativeAdLayout.cpp
   internal/FacebookBannerAd.cpp
   internal/FacebookInterstitialAd.cpp
   internal/FacebookNativeAd.cpp
)

set(LIB_HEADERS
  FacebookAdsBridge.hpp
  FacebookBannerAdSize.hpp
  FacebookNativeAdLayout.hpp
  internal/FacebookBannerAd.hpp
  internal/FacebookInterstitialAd.hpp
  internal/FacebookNativeAd.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})