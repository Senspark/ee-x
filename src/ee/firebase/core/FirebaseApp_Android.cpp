//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/core/FirebaseApp.hpp"

#include <mutex>

#include <jni.h>

#include <firebase/app.h>

#include "ee/core/JniUtils.hpp"

namespace ee {
namespace firebase {
std::mutex global_activity_mutex;
jobject global_activity = nullptr;

extern "C" {
JNIEXPORT void JNICALL Java_com_ee_firebase_core_Firebase_setActivity(
    JNIEnv* env, jobject instance, jobject activity) {
    std::lock_guard<std::mutex> guard(global_activity_mutex);
    if (global_activity != nullptr) {
        env->DeleteGlobalRef(global_activity);
    }
    global_activity = env->NewGlobalRef(activity);
}
} // extern "C"

namespace {
jobject getWindowContext() {
    std::lock_guard<std::mutex> guard(global_activity_mutex);
    return global_activity;
}
} // namespace

using Self = App;

void Self::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    auto options = ::firebase::AppOptions();
    JNIEnv* env = core::JniUtils::getEnv();
    auto app = ::firebase::App::Create(options, env, getWindowContext());
    static_cast<void>(app); // Silence warning.

    initialized = true;
}
} // namespace firebase
} // namespace ee
