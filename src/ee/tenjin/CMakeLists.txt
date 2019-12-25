cmake_minimum_required(VERSION 3.1)

# project(ee_x_tenjin)

include_directories(
  src/ee/tenjin
  src/ee/tenjin/internal
)

set(EE_X_TENJIN_SRC
  src/ee/tenjin/internal/TenjinBridge.cpp
)

set(EE_X_TENJIN_HEADERS
  src/ee/tenjin/internal/TenjinBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})