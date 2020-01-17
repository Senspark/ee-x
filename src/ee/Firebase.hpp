//
//  Firebase.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_HPP_
#define EE_X_FIREBASE_HPP_

#include <ee/Core.hpp>

#include "ee/FirebaseFwd.hpp"

#if __has_include("ee/firebase/core/FirebaseApp.hpp")
#include "ee/firebase/core/FirebaseApp.hpp"
#endif

#if __has_include("ee/firebase/analytics/FirebaseAnalytics.hpp")
#include "ee/firebase/analytics/FirebaseAnalytics.hpp"
#endif

#if __has_include("ee/firebase/remoteconfig/FirebaseRemoteConfig.hpp")
#include "ee/firebase/remoteconfig/FirebaseRemoteConfig.hpp"
#endif

#if __has_include("ee/firebase/storage/FirebaseStorage.hpp")
#include "ee/firebase/storage/FirebaseStorage.hpp"
#endif

#if __has_include("ee/firebase/messaging/FirebaseMessaging.hpp")
#include "ee/firebase/messaging/FirebaseMessaging.hpp"
#endif

#if __has_include("ee/firebase/dynamiclink/FirebaseDynamicLink.hpp")
#include "ee/firebase/dynamiclink/FirebaseDynamicLink.hpp"
#endif

#if __has_include("ee/firebase/performance/FirebasePerformanceBridge.hpp")
#include "ee/firebase/performance/FirebasePerformanceBridge.hpp"
#include "ee/firebase/performance/FirebasePerformanceTrace.hpp"
#endif

#endif /* EE_X_FIREBASE_HPP_ */
