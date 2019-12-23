cmake_minimum_required(VERSION 3.1)

# project(ee_x_appsflyer)

include_directories(
  src/ee/appsflyer
  src/ee/appsflyer/internal
)

set(EE_X_APPSFLYER_SRC
  src/ee/appsflyer/internal/AppsFlyerBridge.cpp
)

set(EE_X_APPSFLYER_HEADERS
  src/ee/appsflyer/IAppsFlyerBridge.hpp
  src/ee/appsflyer/internal/AppsFlyerBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})