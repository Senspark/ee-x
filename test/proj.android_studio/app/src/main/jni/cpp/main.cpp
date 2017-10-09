#include "AppDelegate.hpp"

#include <jni.h>
#include <ee/Core.hpp>

namespace {
std::unique_ptr<AppDelegate> appDelegate;
} // namespace

void cocos_android_app_init(JNIEnv* env) {
    appDelegate.reset(new AppDelegate());

    JavaVM* vm;
    env->GetJavaVM(&vm);
    ee::JniUtils::setVm(vm);
}
