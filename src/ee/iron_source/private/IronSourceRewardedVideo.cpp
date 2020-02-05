//
//  IronSourceRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/iron_source/private/IronSourceRewardedVideo.hpp"

#include <cassert>

#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>

#include "ee/iron_source/IronSourceBridge.hpp"

namespace ee {
namespace iron_source {
using Self = RewardedVideo;

Self::RewardedVideo(const Logger& logger, Bridge* plugin,
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
} // namespace iron_source
} // namespace ee
