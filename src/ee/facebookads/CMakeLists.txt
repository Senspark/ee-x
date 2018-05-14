cmake_minimum_required(VERSION 3.1)

# project(ee_x_facebook_ads)

include_directories(
  src/ee/facebookads/internal
  third_party/nlohmann/include
)

set(EE_X_FACEBOOK_ADS_SRC
   src/ee/facebookads/FacebookAdsBridge.cpp
   src/ee/facebookads/FacebookNativeAdLayout.cpp
   src/ee/facebookads/internal/FacebookBannerAd.cpp
   src/ee/facebookads/internal/FacebookInterstitialAd.cpp
   src/ee/facebookads/internal/FacebookNativeAd.cpp
   src/ee/facebookads/internal/FacebookRewardVideoAd.cpp
)

set(EE_X_FACEBOOK_ADS_HEADERS
  src/ee/facebookads/FacebookAdsBridge.hpp
  src/ee/facebookads/FacebookBannerAdSize.hpp
  src/ee/facebookads/FacebookNativeAdLayout.hpp
  src/ee/facebookads/internal/FacebookBannerAd.hpp
  src/ee/facebookads/internal/FacebookInterstitialAd.hpp
  src/ee/facebookads/internal/FacebookNativeAd.hpp
  src/ee/facebookads/internal/FacebookRewardVideoAd.hpp
)
