cmake_minimum_required(VERSION 3.1)

# project(ee_x_unity_ads)

include_directories(
  third_party/nlohmann/include
  src/ee/unity_ads/internal
  src/ee/unity_ads
)

set(EE_X_UNITY_ADS_SRC
  src/ee/unity_ads/UnityAdsBridge.cpp
  src/ee/unity_ads/internal/UnityInterstitialAd.cpp
  src/ee/unity_ads/internal/UnityRewardedVideo.cpp
)

set(EE_X_UNITY_ADS_HEADERS
  src/ee/unity_ads/UnityAdsBridge.hpp
  src/ee/unity_ads/internal/UnityInterstitialAd.hpp
  src/ee/unity_ads/internal/UnityRewardedVideo.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

# target_link_libraries(${PROJECT_NAME}
#   ee_x_ads
# )