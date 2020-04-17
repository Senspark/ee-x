//
//  UnityInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/unity_ads/private/UnityInterstitialAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>

#include "ee/unity_ads/UnityAdsBridge.hpp"

namespace ee {
namespace unity_ads {
using Self = InterstitialAd;

Self::InterstitialAd(const Logger& logger, Bridge* plugin,
                     const std::string& adId)
    : logger_(logger)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug(__PRETTY_FUNCTION__);
    displayer_ = std::make_unique<ads::AsyncHelper<bool>>();
}

Self::~InterstitialAd() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyInterstitialAd(adId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd(adId_);
}

Task<bool> Self::load() {
    // No effect.
    co_return false;
}

Task<bool> Self::show() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto result = co_await displayer_->process([this] {
        auto&& mediation = ads::MediationManager::getInstance();
        auto successful = mediation.startInterstitialAd([this] { //
            displayer_->resolve(true);
        });
        assert(successful);
        plugin_->showRewardedAd(adId_);
    });
    co_return result;
}

void Self::onFailedToShow(const std::string& message) {
    if (displayer_->isProcessing()) {
        displayer_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onClosed() {
    auto&& mediation = ads::MediationManager::getInstance();

    // Other mediation network.
    // Not sure interstitial ad or rewarded video so check both.
    auto wasInterstitialAd = mediation.setInterstitialAdDone();
    auto wasRewardedVideo = mediation.finishRewardedVideo(false);

    assert(wasInterstitialAd || wasRewardedVideo);
}
} // namespace unity_ads
} // namespace ee
