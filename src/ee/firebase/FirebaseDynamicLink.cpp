//
//  FirebaseMessenging.cpp
//  ee_x
//
//  Created by Zinge on 8/7/17.
//
//

#include <ee/core/Logger.hpp>
#include "ee/firebase/FirebaseDynamicLink.hpp"
#include "ee/firebase/FirebaseApp.hpp"
#include "ee/core/MessageBridge.hpp"

namespace ee {
namespace firebase {
using Self = DynamicLink;
namespace {
constexpr auto k__setupReceiver = "Firebase_DynamicLink_setupReceiver";

constexpr auto k__onReceiveDynamicLink =
    "Firebase_DynamicLink_OnReceiveDynamicLink";
} // namespace

Self::DynamicLink()
    : bridge_(MessageBridge::getInstance()) {
    registerHandlers();
}

Self::~DynamicLink() {
    deRegisterHandlers();
}

void Self::setupReceiver(const OnReceiveLinkCallback& callback) {
    callback_ = callback;
    bridge_.call(k__setupReceiver);
}

void Self::registerHandlers() {
    bridge_.registerHandler(
        [this](const std::string& message) {
            onReceiveDynamicLink(message);
            return "";
        },
        k__onReceiveDynamicLink);
}

void Self::deRegisterHandlers() {
    bridge_.deregisterHandler(k__onReceiveDynamicLink);
}

void Self::onReceiveDynamicLink(std::string link) {
    Logger::getSystemLogger().debug("%s %s", __PRETTY_FUNCTION__, link.c_str());
    callback_(link);
}
} // namespace firebase
} // namespace ee
