#include "AppDelegate.hpp"

#include <jni.h>
#include <ee/Core.hpp>

void cocos_android_app_init(JNIEnv* env) {
    JavaVM* vm;
    env->GetJavaVM(&vm);
    ee::JniUtils::setVm(vm);
}

namespace {
extern "C" {
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = nullptr;
    jint val = vm->GetEnv((void**)&env, JNI_VERSION_1_4);
    cocos_android_app_init(env);
    return  JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL
Java_com_ee_test_android_MyActivity_applicationDidFinishLaunching(
    JNIEnv* env, jobject instance) {
    AppDelegate::getInstance()->applicationDidFinishLaunching();
}

JNIEXPORT void JNICALL
Java_com_ee_test_android_MyActivity_applicationDidEnterBackground(
    JNIEnv* env, jobject instance) {
    AppDelegate::getInstance()->applicationDidEnterBackground();
}

JNIEXPORT void JNICALL
Java_com_ee_test_android_MyActivity_applicationWillEnterForeground(
    JNIEnv* env, jobject instance) {
    AppDelegate::getInstance()->applicationWillEnterForeground();
}
} // extern "C"
} // namespace
