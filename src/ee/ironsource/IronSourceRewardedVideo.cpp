//
//  IronSourceRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/ironsource/IronSourceRewardedVideo.hpp"
#include "ee/ironsource/IronSourceBridge.hpp"

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
    return plugin_->showRewardedVideo(placementId_);
}
} // namespace ironsource
} // namespace ee
