//
//  IronSourceInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/iron_source/private/IronSourceInterstitialAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>

#include "ee/iron_source/IronSourceBridge.hpp"

namespace ee {
namespace iron_source {
using Self = InterstitialAd;

Self::InterstitialAd(const Logger& logger, Bridge* plugin,
                     const std::string& adId)
    : logger_(logger)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug(__PRETTY_FUNCTION__);
}

Self::~InterstitialAd() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyInterstitialAd(adId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->hasInterstitialAd();
}

Task<bool> Self::load() {
    auto result = co_await loader_->process([this] { //
        plugin_->loadInterstitialAd();
    });
    co_return result;
}

Task<bool> Self::show() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto result = co_await displayer_->process([this] {
        auto&& mediation = ads::MediationManager::getInstance();
        auto successful = mediation.startInterstitialAd([this] { //
            displayer_->resolve(true);
        });
        assert(successful);
        plugin_->showInterstitialAd(adId_);
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
        displayer_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onClicked() {
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}

void Self::onClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();

    // Other mediation network.
    auto wasInterstitialAd = mediation.setInterstitialAdDone();
    auto wasRewardedVideo = mediation.finishRewardedVideo(false);

    assert(wasInterstitialAd || wasRewardedVideo);
}
} // namespace iron_source
} // namespace ee
