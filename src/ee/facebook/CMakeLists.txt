cmake_minimum_required(VERSION 3.1)

include_directories(
  src/ee/facebook
  third_party/nlohmann/include
)

set(EE_X_FACEBOOK_SRC
  src/ee/facebook/FacebookGraphRequest.cpp
  src/ee/facebook/FacebookBridge.cpp
  src/ee/facebook/internal/FacebookShareDelegate.cpp
  src/ee/facebook/internal/FacebookLoginDelegate.cpp
  src/ee/facebook/internal/FacebookRequestDelegate.cpp
  src/ee/facebook/internal/FacebookAccessToken.cpp
  src/ee/facebook/internal/FacebookGraphDelegate.cpp
  src/ee/facebook/FacebookRequestContent.cpp
)

set(EE_X_FACEBOOK_HEADERS
  src/ee/facebook/internal/FacebookGraphDelegate.hpp
  src/ee/facebook/internal/FacebookRequestDelegate.hpp
  src/ee/facebook/internal/FacebookAccessToken.hpp
  src/ee/facebook/internal/FacebookShareDelegate.hpp
  src/ee/facebook/FacebookBridge.hpp
  src/ee/facebook/internal/FacebookLoginDelegate.hpp
  src/ee/facebook/FacebookGraphRequest.hpp
  src/ee/facebook/FacebookRequestContent.hpp
  src/ee/facebook/IFacebookDelegate.hpp
  src/ee/facebook/IFacebookAccessToken.hpp
  src/ee/facebook/IFacebookBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})
