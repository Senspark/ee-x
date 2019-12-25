cmake_minimum_required(VERSION 3.1)

# project(ee_x_play)

include_directories(
  src/ee/play
)

set(EE_X_PLAY_SRC
  src/ee/play/PlayBridge.cpp
)

set(EE_X_PLAY_HEADERS
  src/ee/play/PlayBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})
