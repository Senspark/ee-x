//
//  UnityRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/unityads/UnityRewardedVideo.hpp"
#include "ee/unityads/UnityAdsBridge.hpp"

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
    return plugin_->showRewardedVideo(placementId_);
}
} // namespace unityads
} // namespace ee
