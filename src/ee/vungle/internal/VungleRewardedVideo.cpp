//
//  VungleRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/vungle/internal/VungleRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/vungle/VungleBridge.hpp"
#include <cassert>

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
    if (not plugin_->showRewardedVideo()) {
        return false;
    }
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo(this);
    assert(successful);
    return true;
}
} // namespace vungle
} // namespace ee
