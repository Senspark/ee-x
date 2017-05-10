//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/FirebaseApp.hpp"

#include <firebase/app.h>

#if defined(__ANDROID__)
#include "ee/core/JniUtils.hpp"
#elif defined(__APPLE__)
#import <UIKit/UIKit.h>
#endif // __ANDROID__

namespace ee {
namespace firebase {
#if defined(__APPLE__)
WindowContext FirebaseApp::getWindowContext() {
    return [[
        [[UIApplication sharedApplication] keyWindow] rootViewController] view];
}
#endif // __APPLE__

void FirebaseApp::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    auto options = ::firebase::AppOptions();
#if defined(__ANDROID__)
    JNIEnv* env = core::JniUtils::getEnv();
#elif defined(__APPLE__)
    auto app = ::firebase::App::Create(options);
#endif // __ANDROID__

    initialized = true;
}
} // namespace firebase
} // namespace ee
