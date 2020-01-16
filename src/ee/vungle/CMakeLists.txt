cmake_minimum_required(VERSION 3.1)

# project(ee_x_vungle)

include_directories(
  third_party/nlohmann/include
  src/ee/vungle/internal
  src/ee/vungle
)

set(EE_X_VUNGLE_SRC
  src/ee/vungle/VungleBridge.cpp
  src/ee/vungle/internal/VungleRewardedVideo.cpp
)

set(EE_X_VUNGLE_HEADERS
  src/ee/vungle/VungleBridge.hpp
  src/ee/vungle/internal/VungleRewardedVideo.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})