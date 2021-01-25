//
//  BridgeManager.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/core/internal/MessageBridgeCpp.hpp"

#include <ee/nlohmann/json.hpp>

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/internal/MessageBridgeUtils.hpp"

namespace ee {
namespace core {
using Self = MessageBridgeCpp;

Self::MessageBridgeCpp()
    : logger_(Logger::getSystemLogger())
    , callbackCounter_(0) {}

Self::~MessageBridgeCpp() = default;

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

std::string Self::call(const std::string& tag, const std::string& message) {
    return MessageBridgeUtils::call(tag, message);
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
} // namespace core
} // namespace ee
