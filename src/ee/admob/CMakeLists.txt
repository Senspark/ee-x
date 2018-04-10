cmake_minimum_required(VERSION 3.1)

# project(ee_x_admob)

include_directories(
  third_party/nlohmann/include
  src/ee/admob/internal
  src/ee/admob
)

set(EE_X_ADMOB_SRC
   src/ee/admob/AdMobBridge.cpp
   src/ee/admob/AdMobNativeAdLayout.cpp
   src/ee/admob/internal/AdMobBannerAd.cpp
   src/ee/admob/internal/AdMobInterstitialAd.cpp
   src/ee/admob/internal/AdMobNativeAd.cpp
   src/ee/admob/internal/AdMobRewardedVideo.cpp
)

set(EE_X_ADMOB_HEADERS
   src/ee/admob/AdMobBannerAdSize.hpp
   src/ee/admob/AdMobBridge.hpp
   src/ee/admob/AdMobNativeAdLayout.hpp
   src/ee/admob/internal/AdMobBannerAd.hpp
   src/ee/admob/internal/AdMobInterstitialAd.hpp
   src/ee/admob/internal/AdMobNativeAd.hpp
   src/ee/admob/internal/AdMobRewardedVideo.hpp
)

#add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

#target_link_libraries(${PROJECT_NAME} ee_x_ads)
