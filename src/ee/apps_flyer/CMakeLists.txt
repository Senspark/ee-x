cmake_minimum_required(VERSION 3.1)

# project(ee_x_apps_flyer)

include_directories(
  src/ee/apps_flyer
  src/ee/apps_flyer/internal
)

set(EE_X_APPSFLYER_SRC
  src/ee/apps_flyer/internal/AppsFlyerBridge.cpp
)

set(EE_X_APPSFLYER_HEADERS
  src/ee/apps_flyer/IAppsFlyerBridge.hpp
  src/ee/apps_flyer/internal/AppsFlyerBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})