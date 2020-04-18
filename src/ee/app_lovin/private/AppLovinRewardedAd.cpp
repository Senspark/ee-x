//
//  AppLovinRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/app_lovin/private/AppLovinRewardedAd.hpp"

#include <ee/ads/internal/AsyncHelper.hpp>

#include "ee/app_lovin/AppLovinBridge.hpp"

namespace ee {
namespace app_lovin {
using Self = RewardedAd;

Self::RewardedAd(
    const std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>>& displayer,
    AppLovin* plugin)
    : displayer_(displayer)
    , plugin_(plugin) {
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();
}

Self::~RewardedAd() {}

void Self::destroy() {
    plugin_->destroyRewardedAd();
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd();
}

Task<bool> Self::load() {
    auto result = co_await loader_->process(
        [this] { //
            plugin_->loadRewardedAd();
        },
        [](bool result) {
            // OK.
        });
    co_return result;
}

Task<IRewardedAdResult> Self::show() {
    auto result = co_await displayer_->process(
        [this] { //
            plugin_->showRewardedAd();
        },
        [](IRewardedAdResult result) {
            // OK.
        });
    co_return result;
}

void Self::onLoaded() {
    if (loader_->isProcessing()) {
        loader_->resolve(true);
    } else {
        assert(false);
    }
    dispatchEvent([](auto&& observer) {
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

void Self::onClicked() {
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}

void Self::onClosed(bool rewarded) {
    if (displayer_->isProcessing()) {
        displayer_->resolve(rewarded ? IRewardedAdResult::Completed
                                     : IRewardedAdResult::Canceled);
    } else {
        assert(false);
    }
}
} // namespace app_lovin
} // namespace ee
