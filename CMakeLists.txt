cmake_minimum_required(VERSION 3.1)

project(ee_x)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

set(LIB_SRC
   jni/cpp/ee/core/JniUtils.cpp
   jni/cpp/ee/core/internal/JniMethodInfo.cpp
   jni/cpp/ee/core/internal/JniString.cpp
   jni/cpp/ee/core/internal/Logger_Android.cpp
   jni/cpp/ee/core/internal/MessageBridge_Android.cpp
)

set(LIB_HEADERS
    jni/cpp/ee/core/JniUtils.hpp
    jni/cpp/ee/core/internal/JnimethodInfo.hpp
    jni/cpp/ee/core/internal/JniString.hpp
    third_party/nlohmann/include/ee/nlohmann/json.hpp
    src/ee/Vungle.hpp
    src/ee/IronSource.hpp
    src/ee/AdsFwd.hpp
    src/ee/FacebookAds.hpp
    src/ee/Firebase.hpp
    src/ee/AppLovinFwd.hpp
    src/ee/AdMob.hpp
    src/ee/UnityAdsFwd.hpp
    src/ee/FacebookFwd.hpp
    src/ee/UnityAds.hpp
    src/ee/FacebookAdsFwd.hpp
    src/ee/AppLovin.hpp
    src/ee/CrashlyticsFwd.hpp
    src/ee/AdMobFwd.hpp
    src/ee/VungleFwd.hpp
    src/ee/GoogleAnalytics.hpp
    src/ee/CoreFwd.hpp
    src/ee/Macro.hpp
    src/ee/GoogleAnalyticsFwd.hpp
    src/ee/IronSourceFwd.hpp
    src/ee/Crashlytics.hpp
    src/ee/Core.hpp
    src/ee/Notification.hpp
    src/ee/NotificationFwd.hpp
    src/ee/FirebaseFwd.hpp
    src/ee/Facebook.hpp
)

add_subdirectory(src/ee/admob)
add_subdirectory(src/ee/ads)
add_subdirectory(src/ee/applovin)
add_subdirectory(src/ee/core)
add_subdirectory(src/ee/crashlytics)
add_subdirectory(src/ee/facebook)
add_subdirectory(src/ee/firebase)
add_subdirectory(src/ee/google)
add_subdirectory(src/ee/ironsource)
add_subdirectory(src/ee/notification)
#add_subdirectory(src/ee/store)
add_subdirectory(src/ee/unityads)
add_subdirectory(src/ee/vungle)

add_library(${PROJECT_NAME} STATIC ${LIB_SRC} ${LIB_HEADERS})

target_link_libraries(${PROJECT_NAME} 
  ee_x_admob 
  ee_x_ads
  ee_x_applovin
  ee_x_core
  ee_x_crashlytics
  ee_x_facebook_ads
  ee_x_firebase_core
  ee_x_google_analytics
  ee_x_ironsource
  ee_x_notification
#  ee_x_store
  ee_x_unityads
  ee_x_vungle
)

target_include_directories(${PROJECT_NAME} PUBLIC
  src
  jni/cpp
  third_party/nlohmann/include
  third_party/firebase_cpp_sdk/include
)