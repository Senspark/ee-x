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
#include "Logger.hpp"

namespace ee {
namespace core {
namespace {
const Logger& getLogger() {
    static Logger logger{"ee-core"};
    return logger;
}
} // namespace

std::thread::id JniUtils::threadId_{};
JNIEnv* JniUtils::env_ = nullptr;
JavaVM* JniUtils::vm_ = nullptr;

void JniUtils::setVm(JavaVM* vm) noexcept { vm_ = vm; }

void JniUtils::initialize() {
    cacheEnv();
    threadId_ = std::this_thread::get_id();
}

JNIEnv* JniUtils::getEnv() {
    if (env_ == nullptr) {
        throw std::runtime_error{
            "env has not been set, call initialize in AppDelegate.cpp first"};
    }
    if (threadId_ != std::this_thread::get_id()) {
        getLogger().error(__PRETTY_FUNCTION__,
                          ": current thread is not cocos2d-x thread!");
    }
    return env_;
}

void JniUtils::cacheEnv() {
    if (vm_ == nullptr) {
        throw std::runtime_error{
            "java vm has not been set, call setVm in main.cpp first!"};
    }

    jint result = vm_->GetEnv(reinterpret_cast<void**>(&env_), JNI_VERSION_1_4);
    switch (result) {
    case JNI_OK:
        break;
    case JNI_EDETACHED: {
        auto status = vm_->AttachCurrentThread(&env_, nullptr);
        if (status < 0) {
            getLogger().error(
                __PRETTY_FUNCTION__,
                ": failed to get the environment using AttachCurrentThread!");
        }
    }
    case JNI_EVERSION: {
        getLogger().error(__PRETTY_FUNCTION__,
                          ": JNI interface version 1.4 not supported!");
    }
    default: {
        getLogger().error(__PRETTY_FUNCTION__,
                          ": failed to get the environment using GetEnv!");
    }
    }
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
        getLogger().error(__PRETTY_FUNCTION__, ": can not find class ",
                          className);
        return nullptr;
    }

    jmethodID methodId = env->GetStaticMethodID(clazz, methodName, signature);
    checkException();

    if (methodId == nullptr) {
        getLogger().error(__PRETTY_FUNCTION__, ": can not find static method ",
                          methodId, " with signature ", signature);
        return nullptr;
    }

    return JniMethodInfo::create(env, clazz, methodId);
}
} // namespace core
} // namespace ee
