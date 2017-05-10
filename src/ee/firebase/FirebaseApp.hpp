//
//  FirebaseApp.hpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_APP_HPP_
#define EE_X_FIREBASE_APP_HPP_

#include "ee/Macro.hpp"

#if defined(EE_X_ANDROID)
#include <jni.h>
#elif defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
#include <objc/objc.h>
} // extern "C"
#endif // EE_X_ANDROID

#if defined(EE_X_ANDROID)
typedef jobject WindowContext;
#elif defined(EE_X_IOS) || defined(EE_X_OSX)
typedef id WindowContext;
#endif // EE_X_ANDROID

namespace ee {
namespace firebase {
class FirebaseApp {
public:
    static void initialize();

    static WindowContext getWindowContext();
};
} // namespace firebase
} // namespace ee

#endif /* EE_X_FIREBASE_APP_HPP_ */
