//
//  AdMobRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#include <cassert>

#include "AdMobRewardedVideo.hpp"
#include "ee/admob/AdMobBridge.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"

namespace ee {
namespace admob {
using Self = RewardedVideo;

Self::RewardedVideo(const Logger& logger, AdMob* plugin,
                    const std::string& adId)
    : Super(logger)
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    plugin_ = plugin;
    adId_ = adId;
}

Self::~RewardedVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedVideo(adId_);
}

bool Self::isLoaded() const {
    return adId_ == plugin_->currentId_ && plugin_->hasRewardedVideo();
}

void Self::load() {
    plugin_->loadRewardedVideo(adId_);
}

bool Self::show() {
    if (adId_ != plugin_->currentId_ || not plugin_->showRewardedVideo()) {
        return false;
    }
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo([this](bool rewarded) { //
        this->setResult(rewarded);
    });
    assert(successful);
    return true;
}
} // namespace admob
} // namespace ee
