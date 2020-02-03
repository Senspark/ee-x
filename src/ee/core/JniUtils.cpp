//
//  EEJniUtils.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "ee/core/JniUtils.hpp"

#include <cassert>
#include <codecvt>
#include <locale>
#include <pthread.h>

#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"

namespace ee {
namespace core {
namespace {
pthread_key_t key_value;
} // namespace

std::thread::id JniUtils::cocosThreadId_;
bool JniUtils::isCocosThreadMarked_ = false;
JavaVM* JniUtils::vm_ = nullptr;

void JniUtils::setVm(JavaVM* vm) {
    vm_ = vm;
    pthread_key_create(&key_value, &JniUtils::detachCurrentThread);
}

void JniUtils::markCocosThread() noexcept {
    isCocosThreadMarked_ = true;
    cocosThreadId_ = std::this_thread::get_id();
}

JNIEnv* JniUtils::getEnv() {
    // Retrieve the thread local JNIEnv pointer.
    JNIEnv* env = static_cast<JNIEnv*>(pthread_getspecific(key_value));

    if (env == nullptr) {
        // The JNIEnv pointer has not been created.
        // Attempt to create it.
        env = cacheEnv();
    }

    // Check Cocos thread.
//    if (isCocosThreadMarked_ && cocosThreadId_ != std::this_thread::get_id()) {
//        Logger::getSystemLogger().error(
//            "%s: current thread is not cocos2d-x thread!", __PRETTY_FUNCTION__);
//        Logger::getSystemLogger().warn("Backtrace: %s", dumpBacktrace(30).c_str());
//    }

    return env;
}

JNIEnv* JniUtils::cacheEnv() {
    if (vm_ == nullptr) {
        // JavaVM has not been set.
        throw std::runtime_error{
            "java vm has not been set, call setVm in main.cpp first!"};
    }

    JNIEnv* env = nullptr;
    jint result = vm_->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4);

    switch (result) {
    case JNI_OK: {
        // Succeed.
        pthread_setspecific(key_value, static_cast<const void*>(env));
        return env;
    }

    case JNI_EDETACHED: {
        // Thread detached.
        // Attempt to reattach.
        auto status = vm_->AttachCurrentThread(&env, nullptr);
        if (status < 0) {
            Logger::getSystemLogger().error(
                "%s: failed to get the environment using "
                "AttachCurrentThread!",
                __PRETTY_FUNCTION__);
            return nullptr;
        }

        pthread_setspecific(key_value, static_cast<const void*>(env));
        return env;
    }

    case JNI_EVERSION: {
        Logger::getSystemLogger().error(
            "%s: JNI interface version 1.4 not supported!",
            __PRETTY_FUNCTION__);
        return nullptr;
    }

    default: {
        Logger::getSystemLogger().error(
            "%s: failed to get the environment using GetEnv!",
            __PRETTY_FUNCTION__);
        return nullptr;
    }
    }
}

void JniUtils::detachCurrentThread(void*) {
    vm_->DetachCurrentThread();
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

std::unique_ptr<JniString> JniUtils::toJavaString(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    auto u16str = converter.from_bytes(str);
    return toJavaString(u16str);
}

std::unique_ptr<JniString> JniUtils::toJavaString(const std::u16string& str) {
    return JniString::create(getEnv(), str);
}

std::unique_ptr<JniMethodInfo>
JniUtils::getStaticMethodInfo(const char* className, const char* methodName,
                              const char* signature) {
    JNIEnv* env = getEnv();

    jclass clazz = env->FindClass(className);
    checkException();

    if (clazz == nullptr) {
        Logger::getSystemLogger().error("%s: can not find class %s",
                                        __PRETTY_FUNCTION__, className);
        return nullptr;
    }

    jmethodID methodId = env->GetStaticMethodID(clazz, methodName, signature);
    checkException();

    if (methodId == nullptr) {
        Logger::getSystemLogger().error(
            "%s: can not find static method %s in %s with signature %s",
            __PRETTY_FUNCTION__, methodName, className, signature);
        return nullptr;
    }

    return JniMethodInfo::create(env, clazz, methodId);
}
} // namespace core
} // namespace ee
