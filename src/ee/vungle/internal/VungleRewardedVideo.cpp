//
//  VungleRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/vungle/VungleBridge.hpp"
#include "ee/vungle/internal/VungleRewardedVideo.hpp"

namespace ee {
namespace vungle {
using Self = RewardedVideo;

Self::RewardedVideo(Vungle* plugin) {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    plugin_ = plugin;
}

Self::~RewardedVideo() {
    Logger::getSystemLogger().debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedVideo();
    Logger::getSystemLogger().debug("%s: end", __PRETTY_FUNCTION__);
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
