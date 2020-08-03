//
//  Firebase.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_HPP
#define EE_X_FIREBASE_HPP

#ifdef __cplusplus

#include <ee/Core.hpp>

#include "ee/FirebaseFwd.hpp"

#if __has_include("ee/firebase/core/FirebaseCoreBridge.hpp")
#include "ee/firebase/core/FirebaseCoreBridge.hpp"
#endif

#if __has_include("ee/firebase/analytics/FirebaseAnalyticsBridge.hpp")
#include "ee/firebase/analytics/FirebaseAnalyticsBridge.hpp"
#endif

#if __has_include("ee/firebase/crashlytics/FirebaseCrashlyticsBridge.hpp")
#include "ee/firebase/crashlytics/FirebaseCrashlyticsBridge.hpp"
#endif

#if __has_include("ee/firebase/dynamic_link/FirebaseDynamicLinkBridge.hpp")
#include "ee/firebase/dynamic_link/FirebaseDynamicLinkBridge.hpp"
#endif

#if __has_include("ee/firebase/messaging/FirebaseMessagingBridge.hpp")
#include "ee/firebase/messaging/FirebaseMessagingBridge.hpp"
#endif

#if __has_include("ee/firebase/performance/FirebasePerformanceBridge.hpp")
#include "ee/firebase/performance/FirebasePerformanceBridge.hpp"
#include "ee/firebase/performance/FirebasePerformanceTrace.hpp"
#endif

#if __has_include("ee/firebase/remote_config/FirebaseRemoteConfigBridge.hpp")
#include "ee/firebase/remote_config/FirebaseRemoteConfigBridge.hpp"
#endif

#if __has_include("ee/firebase/storage/FirebaseStorageBridge.hpp")
#include "ee/firebase/storage/FirebaseStorageBridge.hpp"
#endif

#endif // __cplusplus

#endif /* EE_X_FIREBASE_HPP */
