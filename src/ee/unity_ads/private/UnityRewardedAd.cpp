//
//  UnityRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/unity_ads/private/UnityRewardedAd.hpp"

#include <cassert>

#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/coroutine/Task.hpp>

#include "ee/unity_ads/UnityAdsBridge.hpp"

namespace ee {
namespace unity_ads {
using Self = RewardedAd;

Self::RewardedAd(
    const Logger& logger,
    const std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>>& displayer,
    Bridge* plugin, const std::string& adId)
    : logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
}

Self::~RewardedAd() {}

void Self::destroy() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    plugin_->destroyRewardedAd(adId_);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd(adId_);
}

Task<bool> Self::load() {
    // No effect.
    co_return false;
}

Task<IRewardedAdResult> Self::show() {
    logger_.debug("%s: adId = %s displaying = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
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
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    dispatchEvent([&](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToShow(const std::string& message) {
    logger_.debug("%s: adId = %s message = %s displaying = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(), message.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(IRewardedAdResult::Failed);
    } else {
        assert(false);
    }
}

void Self::onClosed(bool rewarded) {
    logger_.debug("%s: adId = %s rewarded = %s displaying = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(),
                  core::toString(rewarded).c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(rewarded ? IRewardedAdResult::Completed
                                     : IRewardedAdResult::Canceled);
    } else {
        assert(false);
    }
}
} // namespace unity_ads
} // namespace ee
