//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/FirebaseApp.hpp"

#ifndef __MACH__
#include <firebase/app.h>
#endif // __MACH__

#if defined(__ANDROID__)
#include "ee/core/JniUtils.hpp"
#elif defined(__APPLE__)
#ifndef __MACH__
#import <UIKit/UIKit.h>
#endif // __MACH__
#endif // __ANDROID__

namespace ee {
namespace firebase {
#if defined(__APPLE__)
#ifdef __MACH__
WindowContext FirebaseApp::getWindowContext() {
    return nil;
}
#else  // __MACH__
WindowContext FirebaseApp::getWindowContext() {
    return [[
        [[UIApplication sharedApplication] keyWindow] rootViewController] view];
}
#endif // __MACH__
#endif // __APPLE__

void FirebaseApp::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

#ifndef __MACH__
    auto options = ::firebase::AppOptions();
#if defined(__ANDROID__)
    JNIEnv* env = core::JniUtils::getEnv();
#elif defined(__APPLE__)
    auto app = ::firebase::App::Create(options);
#endif // __ANDROID__
#endif // __MACH__

    initialized = true;
}
} // namespace firebase
} // namespace ee
