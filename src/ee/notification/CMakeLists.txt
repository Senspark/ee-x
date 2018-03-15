cmake_minimum_required(VERSION 3.1)

# project(ee_x_notification)

include_directories(
  src/ee/notification
  third_party/nlohmann/include
)

set(EE_X_NOTIFICATION_SRC
  src/ee/notification/NotificationBridge.cpp
  src/ee/notification/NotificationBuilder.cpp
)

set(EE_X_NOTIFICATION_HEADERS
  src/ee/notification/NotificationBridge.hpp
  src/ee/notification/NotificationBuilder.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})