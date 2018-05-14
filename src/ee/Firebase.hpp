//
//  Firebase.hpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#ifndef EE_X_FIREBASE_HPP_
#define EE_X_FIREBASE_HPP_

#include "ee/FirebaseFwd.hpp"

#if __has_include("ee/firebase/FirebaseApp.hpp")
#include "ee/firebase/FirebaseApp.hpp"
#endif

#if __has_include("ee/firebase/FirebaseAnalytics.hpp")
#include "ee/firebase/FirebaseAnalytics.hpp"
#endif

#if __has_include("ee/firebase/FirebaseRemoteConfig.hpp")
#include "ee/firebase/FirebaseRemoteConfig.hpp"
#endif

#if __has_include("ee/firebase/FirebaseStorage.hpp")
#include "ee/firebase/FirebaseStorage.hpp"
#endif

#if __has_include("ee/firebase/FirebaseMessaging.hpp")
#include "ee/firebase/FirebaseMessaging.hpp"
#endif

#if __has_include("ee/firebase/FirebaseDynamicLink.hpp")
#include "ee/firebase/FirebaseDynamicLink.hpp"
#endif

#endif /* EE_X_FIREBASE_HPP_ */
