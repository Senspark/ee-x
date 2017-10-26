//
//  IronSourceRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/ironsource/IronSourceBridge.hpp"
#include "ee/ironsource/internal/IronSourceRewardedVideo.hpp"

namespace ee {
namespace ironsource {
using Self = RewardedVideo;

Self::RewardedVideo(IronSource* plugin, const std::string& placementId) {
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~RewardedVideo() {
    plugin_->destroyRewardedVideo(placementId_);
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
    auto successful = mediation.registerRewardedVideo(this);
    assert(successful);
    return true;
}
} // namespace ironsource
} // namespace ee
