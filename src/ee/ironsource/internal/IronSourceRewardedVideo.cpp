//
//  IronSourceRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/ironsource/internal/IronSourceRewardedVideo.hpp"

#include <cassert>

#include <ee/ads/MediationManager.hpp>
#include <ee/core/Logger.hpp>

#include "ee/ironsource/IronSourceBridge.hpp"

namespace ee {
namespace ironsource {
using Self = RewardedVideo;

Self::RewardedVideo(const Logger& logger, IronSource* plugin,
                    const std::string& placementId)
    : Super(logger)
    , logger_(logger) {
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~RewardedVideo() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedVideo(placementId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedVideo();
}

void Self::load() {
    // No op.
}

bool Self::show() {
    if (not plugin_->showRewardedVideo(placementId_)) {
        return false;
    }
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo([this](bool rewarded) { //
        this->setResult(rewarded);
    });
    assert(successful);
    return true;
}
} // namespace ironsource
} // namespace ee
