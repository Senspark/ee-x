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
#include "ee/core/Thread.hpp"

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
    , callbackCounter_(0) {}

Self::~MessageBridge() = default;

void Self::registerHandler(const MessageHandler& handler,
                           const std::string& tag) {
    if (handlers_.count(tag) > 0) {
        throw std::runtime_error("Failed to register handler: " + tag);
    }
    handlers_[tag] = handler;
}

void Self::deregisterHandler(const std::string& tag) {
    if (handlers_.count(tag) == 0) {
        throw std::runtime_error("Failed to deregister handler: " + tag);
    }
    handlers_.erase(tag);
}

MessageHandler Self::findHandler(const std::string& tag) const {
    auto iter = handlers_.find(tag);
    if (iter == handlers_.cend()) {
        logger_.error("%s: tag %s doesn't exist!", __PRETTY_FUNCTION__,
                      tag.c_str());
        return nullptr;
    }
    return iter->second;
}

void Self::callCpp(const std::string& tag, const std::string& message) {
    auto handler = findHandler(tag);
    if (handler == nullptr) {
        throw std::runtime_error("Failed to call handler: " + tag);
    }
    handler(message);
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
