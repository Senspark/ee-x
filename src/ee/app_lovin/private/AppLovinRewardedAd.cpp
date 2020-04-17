//
//  AppLovinRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/app_lovin/private/AppLovinRewardedAd.hpp"

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>

#include "ee/app_lovin/AppLovinBridge.hpp"

namespace ee {
namespace app_lovin {
using Self = RewardedAd;

Self::RewardedAd(AppLovin* plugin)
    : plugin_(plugin) {
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();
    displayer_ = std::make_unique<ads::AsyncHelper<IRewardedAdResult>>();
}

Self::~RewardedAd() {
    plugin_->destroyRewardedAd();
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd();
}

Task<bool> Self::load() {
    auto result = co_await loader_->process([this] { //
        plugin_->loadRewardedAd();
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
        plugin_->showRewardedAd();
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
    auto&& mediation = ads::MediationManager::getInstance();
    auto wasRewardedVideo = mediation.finishRewardedVideo(rewarded);
    assert(wasRewardedVideo);
}
} // namespace app_lovin
} // namespace ee
