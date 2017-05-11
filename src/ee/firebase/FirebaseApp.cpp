//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include <mutex>

#include "ee/firebase/FirebaseApp.hpp"
#include "ee/core/JniUtils.hpp"

#include <firebase/app.h>

namespace ee {
namespace firebase {
std::mutex global_activity_mutex;
jobject global_activity = nullptr;

extern "C" {
JNIEXPORT void JNICALL
Java_com_ee_firebase_Firebase_setActivity(JNIEnv* env, jobject instance,
                                          jobject activity) {
    std::lock_guard<std::mutex> guard(global_activity_mutex);
    if (global_activity != nullptr) {
        env->DeleteGlobalRef(global_activity);
    }
    env->NewGlobalRef(global_activity);
}
} // extern "C"

void FirebaseApp::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    auto options = ::firebase::AppOptions();
    JNIEnv* env = core::JniUtils::getEnv();
    auto app = firebase::App::Create(options, env, getWindowContext());

    initialized = true;
}

WindowContext FirebaseApp::getWindowContext() {
    std::lock_guard<std::mutex> guard(global_activity_mutex);
    return global_activity;
}
} // namespace firebase
} // namespace ee
