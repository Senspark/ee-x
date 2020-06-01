//
//  BridgeManager.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/core/internal/MessageBridge.hpp"

#include <cassert>
#include <mutex>

#include <ee/nlohmann/json.hpp>

#include "ee/core/LambdaAwaiter.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/SpinLock.hpp"

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

MessageHandler Self::findHandler(const std::string& tag) {
    std::scoped_lock<SpinLock> lock(*handlerLock_);
    auto iter = handlers_.find(tag);
    if (iter == handlers_.cend()) {
        logger_.error("%s: tag %s doesn't exist!", __PRETTY_FUNCTION__,
                      tag.c_str());
        return nullptr;
    }
    return iter->second;
}
} // namespace core
} // namespace ee
