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
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/coroutine/Task.hpp>

#include "ee/vungle/VungleBridge.hpp"

namespace ee {
namespace vungle {
using Self = RewardedAd;

Self::RewardedAd(
    const Logger& logger,
    const std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>>& displayer,
    Bridge* plugin, const std::string& adId)
    : logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();
}

Self::~RewardedAd() {}

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedAd(adId_);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd(adId_);
}

Task<bool> Self::load() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loader_->isProcessing()).c_str());
    auto result = co_await loader_->process(
        [this] { //
            plugin_->loadRewardedAd(adId_);
        },
        [](bool result) {
            // OK.
        });
    co_return result;
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

void Self::onLoaded() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loader_->isProcessing()).c_str());
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
    logger_.debug("%s: message = %s loading = %s", __PRETTY_FUNCTION__,
                  message.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(false);
    } else {
        assert(false);
    }
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
} // namespace vungle
} // namespace ee
