//
//  CampaignReceiver.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include <cassert>
#include "ee/campaignreceiver/CampaignReceiverBridge.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"

namespace ee {
namespace campaignreceiver {
using Self = CampaignReceiver;

namespace {
// clang-format off
constexpr auto k__onReceivedLink            = "CampaignReceiver_onReceivedLink";
// clang-format on
} // namespace

Self::CampaignReceiver()
    : Self(Logger::getSystemLogger()) {}

Self::CampaignReceiver(const Logger& logger)
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

Self::~CampaignReceiver() {
    bridge_.deregisterHandler(k__onReceivedLink);
}

void Self::initialize(const OnReceivedLinkCallback& callback) {
    callback_ = callback;
}
} // namespace campaignreceiver
} // namespace ee
