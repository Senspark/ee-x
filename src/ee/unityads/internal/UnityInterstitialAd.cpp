//
//  UnityInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/unityads/UnityAdsBridge.hpp"
#include "ee/unityads/internal/UnityInterstitialAd.hpp"

namespace ee {
namespace unityads {
using Self = InterstitialAd;

Self::InterstitialAd(UnityAds* plugin, const std::string& placementId) {
    plugin_ = plugin;
    placementId_ = placementId;
}

Self::~InterstitialAd() {
    plugin_->destroyInterstitialAd(placementId_);
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
    auto successful = mediation.registerInterstitialAd(this);
    assert(successful);
    return true;
}
} // namespace unityads
} // namespace ee
