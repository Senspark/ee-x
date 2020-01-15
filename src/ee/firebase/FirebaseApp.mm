//
//  FirebaseApp.m
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase/FirebaseApp.hpp"

#if defined(EE_X_IOS)
#import <UIKit/UIViewController.h>

#include <firebase/app.h>

#import <ee/core/internal/EEUtils.h>
#endif // EE_X_MOBILE

namespace ee {
namespace firebase {
using Self = App;

void Self::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

#if defined(EE_X_IOS)
    auto options = ::firebase::AppOptions();
    auto app = ::firebase::App::Create(options);
    static_cast<void>(app);
#endif // EE_X_IOS

    initialized = true;
}

#if defined(EE_X_OSX)
WindowContext Self::getWindowContext() {
    return nil;
}
#elif defined(EE_X_IOS)
WindowContext Self::getWindowContext() {
    return [[EEUtils getCurrentRootViewController] view];
}
#endif // EE_X_OSX
} // namespace firebase
} // namespace ee
