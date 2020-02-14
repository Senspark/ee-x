//
//  FirebaseApp.m
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase/core/FirebaseApp.hpp"

#if defined(EE_X_IOS)
#import <FIRApp.h>

#include <firebase/app.h>
#endif // EE_X_IOS

namespace ee {
namespace firebase {
using Self = App;

void Self::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

#if defined(EE_X_IOS)
    [FIRApp configure];

    auto options = ::firebase::AppOptions();
    auto app = ::firebase::App::Create(options);
    static_cast<void>(app);
#endif // EE_X_IOS

    initialized = true;
}
} // namespace firebase
} // namespace ee
