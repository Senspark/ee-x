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

#include "ee/core/SpinLock.hpp"

namespace ee {
namespace core {
using Self = MessageBridge;

Self& Self::getInstance() {
    static Self sharedInstance;
    return sharedInstance;
}

Self::MessageBridge() {
    handlerLock_ = std::make_unique<SpinLock>();
}

Self::~MessageBridge() {}

std::string Self::callCpp(const std::string& tag, const std::string& message) {
    auto handler = findHandler(tag);
    if (not handler) {
        assert(false);
        return "";
    }
    return handler(message);
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
        return nullptr;
    }
    return iter->second;
}
} // namespace core
} // namespace ee
