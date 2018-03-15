cmake_minimum_required(VERSION 3.1)

# project(ee_x_crashlytics)

include_directories(
  src/ee/crashlytics
  third_party/nlohmann/include
)

set(EE_X_CRASHLYTICS_SRC
  src/ee/crashlytics/CrashlyticsBridge.cpp
)

set(EE_X_CRASHLYTICS_HEADERS
  src/ee/crashlytics/CrashlyticsBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})
