cmake_minimum_required(VERSION 3.1)

# project(ee_x_facebook_ads)

include_directories(
  src/ee/facebook_ads/internal
  third_party/nlohmann/include
)

set(EE_X_FACEBOOK_ADS_SRC
   src/ee/facebook_ads/FacebookAdsBridge.cpp
   src/ee/facebook_ads/FacebookNativeAdLayout.cpp
   src/ee/facebook_ads/internal/FacebookBannerAd.cpp
   src/ee/facebook_ads/internal/FacebookInterstitialAd.cpp
   src/ee/facebook_ads/internal/FacebookNativeAd.cpp
   src/ee/facebook_ads/internal/FacebookRewardVideoAd.cpp
)

set(EE_X_FACEBOOK_ADS_HEADERS
  src/ee/facebook_ads/FacebookAdsBridge.hpp
  src/ee/facebook_ads/FacebookBannerAdSize.hpp
  src/ee/facebook_ads/FacebookNativeAdLayout.hpp
  src/ee/facebook_ads/internal/FacebookBannerAd.hpp
  src/ee/facebook_ads/internal/FacebookInterstitialAd.hpp
  src/ee/facebook_ads/internal/FacebookNativeAd.hpp
  src/ee/facebook_ads/internal/FacebookRewardVideoAd.hpp
)
