//
//  CampaignReceiver.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/campaign_receiver/CampaignReceiverBridge.hpp"

#include <cassert>

#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace campaign_receiver {
using Self = Bridge;

namespace {
// clang-format off
constexpr auto k__onReceivedLink            = "CampaignReceiver_onReceivedLink";
constexpr auto k__initialize                = "CampaignReceiver_initialize";
// clang-format on
} // namespace

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.registerHandler(
        [this](const std::string& message) {
            logger_.debug("OnReceivedLinkCallback: %s", message.c_str());
            if (callback_) {
                callback_(message);
            }
            return "";
        },
        k__onReceivedLink);
}

Self::~Bridge() = default;

void Self::destroy() {
    bridge_.deregisterHandler(k__onReceivedLink);
}

void Self::initialize(const OnReceivedLinkCallback& callback) {
#ifdef EE_X_ANDROID
    bridge_.call(k__initialize);
#endif
    callback_ = callback;
}
} // namespace campaign_receiver
} // namespace ee
