//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/FirebaseApp.hpp"
#include "ee/core/JniUtils.hpp"

#include <firebase/app.h>

namespace ee {
void FirebaseApp::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    auto options = ::firebase::AppOptions();
    JNIEnv* env = core::JniUtils::getEnv();
    // FIXME.

    initialized = true;
}
} // namespace firebase
} // namespace ee
