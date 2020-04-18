//
//  IronSourceRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/iron_source/private/IronSourceRewardedAd.hpp"

#include <cassert>

#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/coroutine/Task.hpp>

#include "ee/iron_source/IronSourceBridge.hpp"

namespace ee {
namespace iron_source {
using Self = RewardedAd;

Self::RewardedAd(
    const Logger& logger,
    std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>>& displayer,
    Bridge* plugin, const std::string& adId)
    : logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
}

Self::~RewardedAd() {}

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedAd(adId_);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd();
}

Task<bool> Self::load() {
    // No op.
    co_return false;
}

Task<IRewardedAdResult> Self::show() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto result = co_await displayer_->process(
        [this] { //
            plugin_->showRewardedAd(adId_);
        },
        [](IRewardedAdResult result) {
            // OK.
        });
    co_return result;
}

void Self::onFailedToShow(const std::string& message) {
    logger_.debug("%s: message = %s displaying = %s", __PRETTY_FUNCTION__,
                  message.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(IRewardedAdResult::Failed);
    } else {
        assert(false);
    }
}

void Self::onClicked() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}

void Self::onClosed(bool rewarded) {
    logger_.debug("%s: rewarded = %s displaying = %s", __PRETTY_FUNCTION__,
                  core::toString(rewarded).c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(rewarded ? IRewardedAdResult::Completed
                                     : IRewardedAdResult::Canceled);
    } else {
        assert(false);
    }
}
} // namespace iron_source
} // namespace ee
