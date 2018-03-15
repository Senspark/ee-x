cmake_minimum_required(VERSION 3.1)

include_directories(
  src/ee/facebook
  third_party/nlohmann/include
)

set(EE_X_FACEBOOK_SRC
  src/ee/facebook/FacebookBridge.cpp
)

set(EE_X_FACEBOOK_HEADERS
  src/ee/facebook/FacebookBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})
