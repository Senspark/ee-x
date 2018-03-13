cmake_minimum_required(VERSION 3.1)

project(ee_x_admob)

include_directories(
  ../../
  ../../../third_party/nlohmann/include
  internal
)

set(LIB_SRC
   AdMobBridge.cpp
   AdMobNativeAdLayout.cpp
   internal/AdMobBannerAd.cpp
   internal/AdMobInterstitialAd.cpp
   internal/AdMobNativeAd.cpp
   internal/AdMobRewardedVideo.cpp
)

set(LIB_HEADERS
   AdMobBannerAdSize.hpp
   AdMobBridge.hpp
   AdMobNativeAdLayout.hpp
   internal/AdMobBannerAd.hpp
   internal/AdMobInterstitialAd.hpp
   internal/AdMobNativeAd.hpp
   internal/AdMobRewardedVideo.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

target_link_libraries(${PROJECT_NAME} ee_x_ads)
