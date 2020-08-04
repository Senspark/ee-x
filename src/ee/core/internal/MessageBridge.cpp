//
//  BridgeManager.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/core/internal/MessageBridge.hpp"

#include <mutex>

#include <ee/nlohmann/json.hpp>

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/SpinLock.hpp"
#include "ee/core/internal/SwiftBridge.hpp"

#ifdef EE_X_ANDROID
#include "ee/core/internal/JniMethodInfo.hpp"
#include "ee/core/internal/JniString.hpp"
#include "ee/core/internal/JniUtils.hpp"
#endif // EE_X_ANDROID

namespace ee {
namespace core {
using Self = MessageBridge;

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::MessageBridge()
    : logger_(Logger::getSystemLogger())
    , callbackCounter_(0) {
    handlerLock_ = std::make_unique<SpinLock>();
}

Self::~MessageBridge() = default;

bool Self::registerHandler(const MessageHandler& handler,
                           const std::string& tag) {
    std::scoped_lock<SpinLock> lock(*handlerLock_);
    if (handlers_.count(tag) > 0) {
        assert(false);
        return false;
    }
    handlers_[tag] = handler;
    return true;
}

bool Self::deregisterHandler(const std::string& tag) {
    std::scoped_lock<SpinLock> lock(*handlerLock_);
    if (handlers_.count(tag) == 0) {
        assert(false);
        return false;
    }
    handlers_.erase(tag);
    return true;
}

MessageHandler Self::findHandler(const std::string& tag) const {
    std::scoped_lock<SpinLock> lock(*handlerLock_);
    auto iter = handlers_.find(tag);
    if (iter == handlers_.cend()) {
        logger_.error("%s: tag %s doesn't exist!", __PRETTY_FUNCTION__,
                      tag.c_str());
        return nullptr;
    }
    return iter->second;
}

std::string Self::callCpp(const std::string& tag, const std::string& message) {
    auto handler = findHandler(tag);
    if (not handler) {
        assert(false);
        return "";
    }
    return handler(message);
}

Task<std::string> Self::callAsync(const std::string& tag,
                                  const std::string& message) {
    auto awaiter = LambdaAwaiter<std::string>([this, tag,
                                               message](auto&& resolve) {
        auto callbackTag = tag + std::to_string(callbackCounter_++);
        registerHandler(
            [this, resolve, callbackTag](const std::string& callbackMessage) {
                deregisterHandler(callbackTag);
                resolve(callbackMessage);
                return "";
            },
            callbackTag);
        nlohmann::json json;
        json["callback_tag"] = callbackTag;
        json["message"] = message;
        call(tag, json.dump());
    });
    auto result = co_await awaiter;
    co_return result;
}

#ifdef EE_X_ANDROID
extern "C" {
JNIEXPORT jstring JNICALL
Java_com_ee_internal_MessageBridgeKt_ee_1callCppInternal(JNIEnv* env,
                                                         jclass clazz,
                                                         jstring tag,
                                                         jstring message) {
    auto tag_cpp = env->GetStringUTFChars(tag, nullptr);
    auto message_cpp = env->GetStringUTFChars(message, nullptr);

    auto result = MessageBridge::getInstance().callCpp(tag_cpp, message_cpp);

    env->ReleaseStringUTFChars(tag, tag_cpp);
    env->ReleaseStringUTFChars(message, message_cpp);

    return env->NewStringUTF(result.c_str());
}
} // extern "C"

std::string Self::call(const std::string& tag, const std::string& message) {
    jobject response = JniUtils::callStaticObjectMethod(
        "com/ee/internal/MessageBridgeKt", //
        "ee_staticCall",                   //
        "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
        JniUtils::toJavaString(tag)->get(),
        JniUtils::toJavaString(message)->get());

    jstring response_java = static_cast<jstring>(response);
    auto result = JniUtils::toString(response_java);

    JniUtils::getEnv()->DeleteLocalRef(response);
    return result;
}
#endif // EE_X_ANDROID

#if defined(EE_X_IOS) || defined(EE_X_OSX)
extern "C" {
char* ee_callCppInternal(const char* tag, const char* message) {
    auto&& bridge = ee::MessageBridge::getInstance();
    auto result = bridge.callCpp(tag, message);
    auto result_chars = strdup(result.c_str());
    return result_chars;
}

char* ee_staticCall(const char* tag, const char* message);
} // extern "C"

std::string Self::call(const std::string& tag, const std::string& message) {
    auto response_swift = ee_staticCall(tag.c_str(), message.c_str());
    if (response_swift == nullptr) {
        return "";
    }
    auto result = std::string(response_swift);
    std::free(response_swift);
    return result;
}
#endif // defined(EE_X_IOS) || defined(EE_X_OSX)
} // namespace core
} // namespace ee
