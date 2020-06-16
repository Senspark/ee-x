//
//  FirebaseApp.cpp
//  ee_firebase
//
//  Created by Zinge on 5/9/17.
//
//

#include "ee/firebase/core/FirebaseCore.hpp"

#include <firebase/app.h>

#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniUtils.hpp"

namespace ee {
namespace firebase {
namespace {
jobject getWindowContext() {
    // Fix issue where IPlugin::onCreate is called before Activity.onCreate.
    auto methodInfo = JniUtils::getStaticMethodInfo(
        "com/ee/firebase/core/FirebaseCore", "staticGetActivity",
        "()Ljava/lang/Object;");
    if (methodInfo == nullptr) {
        throw std::runtime_error("Method not found!");
    }
    jobject response = methodInfo->getEnv()->CallStaticObjectMethod(
        methodInfo->getClass(), methodInfo->getMethodId());
    return response;
}
} // namespace

using Self = Core;

void Self::initialize() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    auto options = ::firebase::AppOptions();
    JNIEnv* env = JniUtils::getEnv();
    auto app = ::firebase::App::Create(options, env, getWindowContext());
    static_cast<void>(app); // Silence warning.

    initialized = true;
}
} // namespace firebase
} // namespace ee
