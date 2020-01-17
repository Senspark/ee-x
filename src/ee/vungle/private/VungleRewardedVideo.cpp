//
//  VungleRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/vungle/private/VungleRewardedVideo.hpp"

#include <cassert>

#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>

#include "ee/vungle/VungleBridge.hpp"

namespace ee {
namespace vungle {
using Self = RewardedVideo;

Self::RewardedVideo(const Logger& logger, Vungle* plugin,
                    const std::string& placementId)
    : Super(logger)
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~RewardedVideo() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedVideo(placementId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedVideo(placementId_);
}

void Self::load() {
    return plugin_->loadVideoAd(placementId_);
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
} // namespace vungle
} // namespace ee
