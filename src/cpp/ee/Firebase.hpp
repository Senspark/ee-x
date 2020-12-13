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

#if __has_include("ee/firebase_core/FirebaseCoreBridge.hpp")
#include "ee/firebase_core/FirebaseCoreBridge.hpp"
#endif

#if __has_include("ee/firebase_analytics/FirebaseAnalyticsBridge.hpp")
#include "ee/firebase_analytics/FirebaseAnalyticsBridge.hpp"
#endif

#if __has_include("ee/firebase_dynamic_link/FirebaseDynamicLinkBridge.hpp")
#include "ee/firebase_dynamic_link/FirebaseDynamicLinkBridge.hpp"
#endif

#if __has_include("ee/firebase_messaging/FirebaseMessagingBridge.hpp")
#include "ee/firebase_messaging/FirebaseMessagingBridge.hpp"
#endif

#if __has_include("ee/firebase_remote_config/FirebaseRemoteConfigBridge.hpp")
#include "ee/firebase_remote_config/FirebaseRemoteConfigBridge.hpp"
#endif

#if __has_include("ee/firebase_storage/FirebaseStorageBridge.hpp")
#include "ee/firebase_storage/FirebaseStorageBridge.hpp"
#endif

#endif // __cplusplus

#endif /* EE_X_FIREBASE_HPP */
