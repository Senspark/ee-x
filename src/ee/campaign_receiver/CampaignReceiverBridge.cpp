//
//  CampaignReceiver.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/campaign_receiver/CampaignReceiverBridge.hpp"

#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

namespace ee {
namespace campaign_receiver {
using Self = Bridge;

namespace {
// clang-format off
const std::string kPrefix  = "CampaignReceiverBridge";
const auto kInitialize     = kPrefix + "Initialize";
const auto kOnReceivedLink = kPrefix + "OnReceivedLink";
// clang-format on
} // namespace

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    PluginManager::addPlugin(Plugin::CampaignReceiver);

    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] {
                logger_.debug("OnReceivedLinkCallback: %s", message.c_str());
                if (callback_) {
                    callback_(message);
                }
            });
            return "";
        },
        kOnReceivedLink);
}

Self::~Bridge() = default;

void Self::destroy() {
    bridge_.deregisterHandler(kOnReceivedLink);
    PluginManager::removePlugin(Plugin::CampaignReceiver);
}

void Self::initialize(const OnReceivedLinkCallback& callback) {
#ifdef EE_X_ANDROID
    bridge_.call(kInitialize);
#endif // EE_X_ANDROID
    callback_ = callback;
}
} // namespace campaign_receiver
} // namespace ee
