//
//  FirebaseApp.hpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#ifndef EE_X_FIREBASE_APP_HPP_
#define EE_X_FIREBASE_APP_HPP_

#if defined(__ANDROID__)
#include <jni.h>
#elif defined(__APPLE__)
extern "C" {
#include <objc/objc.h>
} // extern "C"
#endif // __ANDROID

#if defined(__ANDROID__)
typedef jobject WindowContext;
#elif defined(__APPLE__)
typedef id WindowContext;
#endif // __ANDROID__

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
