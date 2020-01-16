cmake_minimum_required(VERSION 3.1)

# project(ee_x_ironsource)

include_directories(
  third_party/nlohmann/include
  src/ee/ironsource/internal
  src/ee/ironsource
)

set(EE_X_IRONSOURCE_SRC
  src/ee/ironsource/IronSourceBridge.cpp
  src/ee/ironsource/internal/IronSourceInterstitialAd.cpp
  src/ee/ironsource/internal/IronSourceRewardedVideo.cpp
)

set(EE_X_IRONSOURCE_HEADERS
  src/ee/ironsource/IronSourceBridge.hpp
  src/ee/ironsource/internal/IronSourceInterstitialAd.hpp
  src/ee/ironsource/internal/IronSourceRewardedVideo.hpp
)
