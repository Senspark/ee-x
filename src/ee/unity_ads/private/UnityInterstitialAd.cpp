//
//  UnityInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/unity_ads/private/UnityInterstitialAd.hpp"

#include <cassert>

#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/core/Logger.hpp>
#include <ee/coroutine/Task.hpp>

#include "ee/unity_ads/UnityAdsBridge.hpp"

namespace ee {
namespace unity_ads {
using Self = InterstitialAd;

Self::InterstitialAd(const Logger& logger,
                     const std::shared_ptr<ads::IAsyncHelper<bool>>& displayer,
                     Bridge* plugin, const std::string& adId)
    : logger_(logger)
    , displayer_(displayer)
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
    return plugin_->hasRewardedAd(adId_);
}

Task<bool> Self::load() {
    // No effect.
    co_return false;
}

Task<bool> Self::show() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto result = co_await displayer_->process(
        [this] { //
            plugin_->showRewardedAd(adId_);
        },
        [](bool result) {
            // OK.
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
    if (displayer_->isProcessing()) {
        displayer_->resolve(true);
    } else {
        assert(false);
    }
}
} // namespace unity_ads
} // namespace ee
