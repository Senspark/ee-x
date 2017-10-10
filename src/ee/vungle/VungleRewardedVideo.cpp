//
//  VungleRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/vungle/VungleRewardedVideo.hpp"
#include "ee/vungle/VungleBridge.hpp"

namespace ee {
namespace vungle {
using Self = RewardedVideo;

Self::RewardedVideo(Vungle* plugin) {
    plugin_ = plugin;
}

Self::~RewardedVideo() {
    plugin_->destroyRewardedVideo();
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedVideo();
}

void Self::load() {
    // No op.
}

bool Self::show() {
    return plugin_->showRewardedVideo();
}
} // namespace vungle
} // namespace ee
