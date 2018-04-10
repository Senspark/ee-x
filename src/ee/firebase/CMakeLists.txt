cmake_minimum_required(VERSION 3.1)

# project(ee_x_firebase)

include_directories(
    src/ee/firebase
    third_party/firebase_cpp_sdk/include
)

set(EE_X_FIREBASE_SRC
   src/ee/firebase/FirebaseAnalytics.cpp
   src/ee/firebase/FirebaseMessaging.cpp
   src/ee/firebase/FirebaseRemoteConfig.cpp
   src/ee/firebase/FirebaseStorage.cpp
)

set(EE_X_FIREBASE_HEADERS
   src/ee/firebase/FirebaseAnalytics.hpp
   src/ee/firebase/FirebaseApp.hpp
   src/ee/firebase/FirebaseMessaging.hpp
   src/ee/firebase/FirebaseRemoteConfig.hpp
   src/ee/firebase/FirebaseScheduler_inl.hpp
   src/ee/firebase/FirebaseScheduler.hpp
   src/ee/firebase/FirebaseStorage.hpp
)

# add_library(${PROJECT_NAME} ${LIB_SRC} ${LIB_HEADERS})

# target_link_libraries(${PROJECT_NAME}
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libapp.a
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libanalytics.a
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libmessaging.a
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libremote_config.a
#     ${CMAKE_CURRENT_SOURCE_DIR}/../../../third_party/firebase_cpp_sdk/libs/android/${ANDROID_ABI}/libstorage.a
# )
