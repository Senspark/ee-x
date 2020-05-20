#include <jni.h>

#include <ee/Core.hpp>

#include "AppDelegate.hpp"

namespace {
std::unique_ptr<eetest::AppDelegate> appDelegate;
} // namespace

void cocos_android_app_init(JNIEnv* env) {
    appDelegate.reset(new eetest::AppDelegate());

    JavaVM* vm;
    env->GetJavaVM(&vm);
    ee::JniUtils::setVm(vm);
}
