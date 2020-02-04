cmake_minimum_required(VERSION 3.1)

# project(ee_x_campaignreceiver)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(
  src/ee/campaign_receiver
)

set(EE_X_CAMPRECEIVER_SRC
  src/ee/campaign_receiver/CampaignReceiverBridge.cpp
)

set(EE_X_CAMPRECEIVER_HEADERS
  src/ee/campaign_receiver/CampaignReceiverBridge.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})
