//
//  EEJniUtils.cpp
//  ee-library
//
//  Created by enrevol on 10/31/15.
//
//

#include "ee/core/internal/JniUtils.hpp"

#include <cassert>
#include <codecvt>
#include <locale>

#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/JniExceptionChecker.hpp"
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"

namespace ee {
namespace core {
using Self = JniUtils;

JavaVM* Self::vm_ = nullptr;

void Self::setVm(JavaVM* vm) {
    vm_ = vm;
}

JavaVM* Self::getVm() {
    return vm_;
}

namespace {
JNIEnv* getEnvInternal(JavaVM* vm) {
    // https://stackoverflow.com/questions/30026030/what-is-the-best-way-to-save-jnienv
    JNIEnv* env;
    jint result = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4);

    switch (result) {
    case JNI_OK: {
        return env;
    }

    case JNI_EDETACHED: {
        // Thread detached.
        // Attempt to reattach.
        auto status = vm->AttachCurrentThread(&env, nullptr);
        if (status < 0) {
            Logger::getSystemLogger().error(
                "%s: failed to get the environment using "
                "AttachCurrentThread!",
                __PRETTY_FUNCTION__);
            return nullptr;
        }
        class ThreadExiter {
        public:
            ThreadExiter(JavaVM* vm)
                : vm_(vm) {}

            ~ThreadExiter() { //
                vm_->DetachCurrentThread();
            }

        private:
            JavaVM* vm_;
        };
        thread_local ThreadExiter exiter(vm);
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
} // namespace

JNIEnv* Self::getEnv() {
    thread_local auto env = getEnvInternal(getVm());
    return env;
}

std::string Self::toString(jstring str) {
    JNIEnv* env = getEnv();
    JniExceptionChecker checker(env);
    if (str == nullptr) {
        return "";
    }
    jboolean isCopy;
    const char* chars = env->GetStringUTFChars(str, &isCopy);
    std::string result{chars};
    env->ReleaseStringUTFChars(str, chars);
    return result;
}

std::unique_ptr<JniString> Self::toJavaString(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    auto u16str = converter.from_bytes(str);
    return toJavaString(u16str);
}

std::unique_ptr<JniString> Self::toJavaString(const std::u16string& str) {
    return JniString::create(getEnv(), str);
}

std::unique_ptr<JniMethodInfo>
Self::getStaticMethodInfo(const char* className, const char* methodName,
                          const char* signature) {
    JNIEnv* env = getEnv();
    JniExceptionChecker checker(env);
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        Logger::getSystemLogger().error("%s: can not find class %s",
                                        __PRETTY_FUNCTION__, className);
        return nullptr;
    }
    jmethodID methodId = env->GetStaticMethodID(clazz, methodName, signature);
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
