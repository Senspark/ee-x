cmake_minimum_required(VERSION 3.1)

# project(ee_x_unityads)

include_directories(
  third_party/nlohmann/include
  src/ee/unityads/internal
  src/ee/unityads
)

set(EE_X_UNITY_ADS_SRC
  src/ee/unityads/UnityAdsBridge.cpp
  src/ee/unityads/internal/UnityInterstitialAd.cpp
  src/ee/unityads/internal/UnityRewardedVideo.cpp
)

set(EE_X_UNITY_ADS_HEADERS
  src/ee/unityads/UnityAdsBridge.hpp
  src/ee/unityads/internal/UnityInterstitialAd.hpp
  src/ee/unityads/internal/UnityRewardedVideo.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

# target_link_libraries(${PROJECT_NAME}
#   ee_x_ads
# )