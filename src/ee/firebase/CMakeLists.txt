cmake_minimum_required(VERSION 3.1)

project(ee_x_firebase_core)

include_directories(
    ../../
    ../../../third_party/firebase_cpp_sdk/include
)

set(LIB_SRC
   FirebaseAnalytics.cpp
   FirebaseMessaging.cpp
   FirebaseRemoteConfig.cpp
   FirebaseStorage.cpp
)

set(LIB_HEADERS
   FirebaseAnalytics.hpp
   FirebaseApp.hpp
   FirebaseMessaging.hpp
   FirebaseRemoteConfig.hpp
   FirebaseScheduler_inl.hpp
   FirebaseScheduler.hpp
   FirebaseStorage.hpp
)

add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libapp.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libanalytics.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libmessaging.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libremote_config.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libstorage.a
)
