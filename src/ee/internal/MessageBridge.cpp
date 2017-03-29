//
//  BridgeManager.cpp
//  ee_core
//
//  Created by Zinge on 3/28/17.
//
//

#include "ee/internal/MessageBridge.hpp"

namespace ee {
MessageBridge& MessageBridge::getInstance() {
    static MessageBridge sharedInstance;
    return sharedInstance;
}

MessageBridge::MessageBridge() {
}

MessageBridge::~MessageBridge() {
}

std::string MessageBridge::call(const std::string& tag) {
    return call(tag, "");
}
} // namespace ee
