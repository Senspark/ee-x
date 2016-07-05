//
//  EEJniUtils.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include <cassert>

#include "JniUtils.hpp"
#include "JniMethodInfo.hpp"
#include "JniString.hpp"

namespace ee {
namespace core {
std::thread::id JniUtils::threadId_{};
JNIEnv* JniUtils::env_ = nullptr;
JavaVM* JniUtils::vm_ = nullptr;

void JniUtils::initialize(JavaVM* vm, JNIEnv* env) noexcept {
    vm_ = vm;
    env_ = env;
    threadId_ = std::this_thread::get_id();
}

JNIEnv* JniUtils::getEnv() {
    if (env_ == nullptr) {
        throw std::runtime_error{"Call initialize first!"};
    }
    assert(threadId_ == std::this_thread::get_id());
    return env_;
}

void JniUtils::checkException() {
    JNIEnv* env = getEnv();
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

std::string JniUtils::toString(jstring str) {
    if (str == nullptr) {
        return "";
    }

    JNIEnv* env = getEnv();
    jboolean isCopy;
    const char* chars = env->GetStringUTFChars(str, &isCopy);
    std::string result{chars};
    env->ReleaseStringUTFChars(str, chars);
    checkException();
    return result;
}

std::unique_ptr<JniString> JniUtils::toJavaString(const char* str) {
    return JniString::create(getEnv(), str);
}

std::unique_ptr<JniMethodInfo>
JniUtils::getStaticMethodInfo(const char* className, const char* methodName,
                              const char* signature) {
    JNIEnv* env = getEnv();

    jclass clazz = env->FindClass(className);
    checkException();

    if (clazz == nullptr) {
        return nullptr;
    }

    jmethodID methodId = env->GetStaticMethodID(clazz, methodName, signature);
    checkException();

    if (methodId == nullptr) {
        return nullptr;
    }

    return JniMethodInfo::create(env, clazz, methodId);
}
} // namespace core
} // namespace ee
