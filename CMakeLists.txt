cmake_minimum_required(VERSION 3.1)

project(ee_x)

add_subdirectory(src/ee/admob)
add_subdirectory(src/ee/ads)
add_subdirectory(src/ee/app_lovin)
add_subdirectory(src/ee/apps_flyer)
add_subdirectory(src/ee/campaign_receiver)
add_subdirectory(src/ee/cocos)
add_subdirectory(src/ee/core)
add_subdirectory(src/ee/crashlytics)
add_subdirectory(src/ee/facebook)
add_subdirectory(src/ee/facebook_ads)
add_subdirectory(src/ee/firebase/analytics)
add_subdirectory(src/ee/firebase/core)
add_subdirectory(src/ee/firebase/dynamic_link)
add_subdirectory(src/ee/firebase/messaging)
add_subdirectory(src/ee/firebase/performance)
add_subdirectory(src/ee/firebase/remote_config)
add_subdirectory(src/ee/firebase/storage)
add_subdirectory(src/ee/google)
add_subdirectory(src/ee/iron_source)
add_subdirectory(src/ee/notification)
add_subdirectory(src/ee/play)
add_subdirectory(src/ee/recorder)
add_subdirectory(src/ee/tenjin)
add_subdirectory(src/ee/twitter)
add_subdirectory(src/ee/unity_ads)
add_subdirectory(src/ee/vungle)

add_subdirectory(third_party/firebase_cpp_sdk)
add_subdirectory(third_party/jansson)
add_subdirectory(third_party/soomla/SoomlaCocos2dxCore)
add_subdirectory(third_party/soomla/SoomlaCocos2dxStore)

# set_target_properties(${PROJECT_NAME}
#   PROPERTIES
#   ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
# )