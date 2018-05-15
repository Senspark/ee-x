//
//  UnityInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/unityads/UnityAdsBridge.hpp"
#include "ee/unityads/internal/UnityInterstitialAd.hpp"

namespace ee {
namespace unityads {
using Self = InterstitialAd;

Self::InterstitialAd(Logger& logger, UnityAds* plugin,
                     const std::string& placementId)
    : logger_(logger) {
    logger_.debug(__PRETTY_FUNCTION__);
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~InterstitialAd() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyInterstitialAd(placementId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
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
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startInterstitialAd([this] { //
        setDone();
    });
    assert(successful);
    return true;
}
} // namespace unityads
} // namespace ee
