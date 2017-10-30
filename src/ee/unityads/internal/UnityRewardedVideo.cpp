//
//  UnityRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/unityads/UnityAdsBridge.hpp"
#include "ee/unityads/internal/UnityRewardedVideo.hpp"

namespace ee {
namespace unityads {
using Self = RewardedVideo;

Self::RewardedVideo(UnityAds* plugin, const std::string& placementId) {
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~RewardedVideo() {
    plugin_->destroyRewardedVideo(placementId_);
}

bool Self::isLoaded() const {
    return plugin_->isRewardedVideoReady(placementId_);
}

void Self::load() {
    // No op.
}

bool Self::show() {
    if (not plugin_->showRewardedVideo(placementId_)) {
        return false;
    }
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo(this);
    assert(successful);
    return true;
}
} // namespace unityads
} // namespace ee
