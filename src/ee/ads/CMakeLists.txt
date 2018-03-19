cmake_minimum_required(VERSION 3.1)

#project(ee_x_ads)

include_directories(
  third_party/nlohmann/include
  src/ee/ads/internal
  src/ee/ads
)

set(EE_X_ADS_SRC
  src/ee/ads/AdViewInterface.cpp
  src/ee/ads/InterstitialAdInterface.cpp
  src/ee/ads/MultiAdView.cpp
  src/ee/ads/MultiInterstitialAd.cpp
  src/ee/ads/MultiRewardedVideo.cpp
  src/ee/ads/RewardedVideoInterface.cpp
  src/ee/ads/internal/AdViewBridgeHelper.cpp
  src/ee/ads/internal/AdViewHelper.cpp
)

set(EE_X_ADS_HEADERS
  src/ee/ads/AdViewInterface.hpp
  src/ee/ads/InterstitialAdInterface.hpp
  src/ee/ads/MultiAdView.hpp
  src/ee/ads/MultiInterstitialAd.hpp
  src/ee/ads/MultiRewardedVideo.hpp
  src/ee/ads/NullAdView.hpp
  src/ee/ads/NullInterstitialAd.hpp
  src/ee/ads/NullRewardedVideo.hpp
  src/ee/ads/RewardedVideoInterface.hpp
  src/ee/ads/internal/AdViewBridgeHelper.hpp
  src/ee/ads/internal/AdViewHelper.hpp
  src/ee/ads/internal/MediationManager.hpp
)

#add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

