//
//  VungleRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/vungle/private/VungleRewardedAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/coroutine/Task.hpp>

#include "ee/vungle/VungleBridge.hpp"

namespace ee {
namespace vungle {
using Self = RewardedAd;

Self::RewardedAd(const Logger& logger, Bridge* plugin, const std::string& adId)
    : logger_(logger)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();
    displayer_ = std::make_unique<ads::AsyncHelper<IRewardedAdResult>>();
}

Self::~RewardedAd() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedAd(adId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd(adId_);
}

Task<bool> Self::load() {
    auto result = co_await loader_->process([this] { //
        plugin_->loadRewardedAd(adId_);
    });
    co_return result;
}

Task<IRewardedAdResult> Self::show() {
    auto result = co_await displayer_->process([this] {
        auto&& mediation = ads::MediationManager::getInstance();
        auto successful = mediation.startRewardedVideo([this](bool rewarded) {
            displayer_->resolve(rewarded ? IRewardedAdResult::Completed
                                         : IRewardedAdResult::Canceled);
        });
        assert(successful);
        plugin_->showRewardedAd(adId_);
    });
    co_return result;
}

void Self::onLoaded() {
    if (loader_->isProcessing()) {
        loader_->resolve(true);
    } else {
        assert(false);
    }
    dispatchEvent([&](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(const std::string& message) {
    if (loader_->isProcessing()) {
        loader_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onFailedToShow(const std::string& message) {
    if (displayer_->isProcessing()) {
        displayer_->resolve(IRewardedAdResult::Failed);
    } else {
        assert(false);
    }
}

void Self::onClosed(bool rewarded) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();

    // Other mediation network.
    auto wasInterstitialAd = mediation.setInterstitialAdDone();
    auto wasRewardedVideo = mediation.finishRewardedVideo(rewarded);

    assert(wasInterstitialAd || wasRewardedVideo);
}
} // namespace vungle
} // namespace ee
