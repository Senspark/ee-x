//
//  FirebaseApp.m
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase/FirebaseApp.hpp"

#if defined(EE_X_IOS)
#include <firebase/app.h>
#import <UIKit/UIKit.h>
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

#if defined(EE_X_IOS)
    auto options = ::firebase::AppOptions();
    auto app = ::firebase::App::Create(options);
#endif // EE_X_IOS

    initialized = true;
}
} // namespace firebase
} // namespace ee
