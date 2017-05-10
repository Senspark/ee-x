//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/FirebaseApp.hpp"

#if defined(EE_X_MOBILE)
#include <firebase/app.h>
#if defined(EE_X_ANDROID)
#include "ee/core/JniUtils.hpp"
#else // EE_X_ANDROID
#import <UIKit/UIKit.h>
#endif // EE_X_ANDROID
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
#if defined(EE_X_OSX)
WindowContext FirebaseApp::getWindowContext() {
    return nil;
}
#elif defined(EE_X_IOS)
WindowContext FirebaseApp::getWindowContext() {
    return [[
        [[UIApplication sharedApplication] keyWindow] rootViewController] view];
}
#endif // EE_X_OSX

void FirebaseApp::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

#if defined(EE_X_MOBILE)
    auto options = ::firebase::AppOptions();
#if defined(EE_X_ANDROID)
    JNIEnv* env = core::JniUtils::getEnv();
// FIXME.
#else  // EE_X_ANDROID
    auto app = ::firebase::App::Create(options);
#endif // EE_X_ANDROID
#endif // EE_X_MOBILE

    initialized = true;
}
} // namespace firebase
} // namespace ee
