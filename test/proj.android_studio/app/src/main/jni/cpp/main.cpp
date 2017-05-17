#include "AppDelegate.hpp"

#include <jni.h>
#include <ee/Core.hpp>

void cocos_android_app_init(JNIEnv* env) {
    JavaVm* vm;
    env->GetJavaVm(&vm);
    ee::JniUtils::setVm(vm);
}

namespace {
extern "C" {
JNIEXPORT void JNICALL
Java_com_ee_test_android_AppActivity_applicationDidFinishLaunching(
    JNIEnv* env, jobject instance) {
    AppDelegate::getInstance()->applicationDidFinishLaunching();
}

JNIEXPORT void JNICALL
Java_com_ee_test_android_AppActivity_applicationDidEnterBackground(
    JNIEnv* env, jobject instance) {
    AppDelegate::getInstance()->applicationDidEnterBackground();
}

JNIEXPORT void JNICALL
Java_com_ee_test_android_AppActivity_applicationWillEnterForeground(
    JNIEnv* env, jobject instance) {
    AppDelegate::getInstance()->applicationWillEnterForeground();
}
} // extern "C"
} // namespace
