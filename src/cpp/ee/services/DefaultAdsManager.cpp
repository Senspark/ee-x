#include "ee/services/DefaultAdsManager.hpp"

#include <cassert>

#include <ee/cocos/CocosAdView.hpp>
#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

#include "ee/services/AdResult.hpp"
#include "ee/services/internal/AdsConfig.hpp"
#include "ee/services/internal/GenericAd.hpp"
#include "ee/services/internal/LazyAdView.hpp"

namespace ee {
namespace services {
using Self = DefaultAdsManager;

Self::DefaultAdsManager(const std::string& configJson) {
    config_ = AdsConfig::parse(configJson);
    handle_ = std::make_unique<ObserverHandle>();
}

Task<bool> Self::initialize() {
    bannerAd_ = std::make_shared<LazyAdView>();
    co_await config_->initialize();
    initializeBannerAd();
    initializeAppOpenAd();
    initializeInterstitialAd();
    initializeRewardedAd();
    initialized_ = true;
    co_return true;
}

void Self::initializeBannerAd() {
    auto ad =
        std::dynamic_pointer_cast<IAdView>(config_->createAd(AdFormat::Banner));
    if (ad == nullptr) {
        return;
    }
    bannerAd_->setAd(ad);
    (*handle_) //
        .bind(*bannerAd_)
        .addObserver({
            .onClicked =
                [this] {
                    dispatchEvent([](auto&& observer) {
                        if (observer.onClicked) {
                            observer.onClicked();
                        }
                    });
                },
        });
}

void Self::initializeAppOpenAd() {
    auto ad = std::dynamic_pointer_cast<GenericAd>(
        config_->createAd(AdFormat::AppOpen));
    if (ad == nullptr) {
        return;
    }
    appOpenAd_ = ad;
    (*handle_) //
        .bind(*appOpenAd_)
        .addObserver({
            .onClicked =
                [this] {
                    dispatchEvent([](auto&& observer) {
                        if (observer.onClicked) {
                            observer.onClicked();
                        }
                    });
                },
        });
}

void Self::initializeInterstitialAd() {
    auto ad = std::dynamic_pointer_cast<GenericAd>(
        config_->createAd(AdFormat::Interstitial));
    if (ad == nullptr) {
        return;
    }
    interstitialAd_ = ad;
    (*handle_) //
        .bind(*interstitialAd_)
        .addObserver({
            .onClicked =
                [this] {
                    dispatchEvent([](auto&& observer) {
                        if (observer.onClicked) {
                            observer.onClicked();
                        }
                    });
                },
        });
}

void Self::initializeRewardedAd() {
    auto ad = std::dynamic_pointer_cast<GenericAd>(
        config_->createAd(AdFormat::Rewarded));
    if (ad == nullptr) {
        return;
    }
    rewardedAd_ = ad;
    (*handle_) //
        .bind(*rewardedAd_)
        .addObserver({
            .onClicked =
                [this] {
                    dispatchEvent([](auto&& observer) {
                        if (observer.onClicked) {
                            observer.onClicked();
                        }
                    });
                },
        });
}

bool Self::isBannerAdVisible() const {
    return bannerAd_->isVisible();
}

void Self::setBannerAdVisible(bool visible) {
    bannerAd_->setVisible(visible);
}

std::pair<float, float> Self::getBannerAdAnchor() const {
    return bannerAd_->getAnchor();
}

void Self::setBannerAdAnchor(float x, float y) {
    bannerAd_->setAnchor(x, y);
}

std::pair<float, float> Self::getBannerAdPosition() const {
    return bannerAd_->getPosition();
}

void Self::setBannerAdPosition(float x, float y) {
    bannerAd_->setPosition(x, y);
}

std::pair<float, float> Self::getBannerAdSize() const {
    return bannerAd_->getSize();
}

void Self::setBannerAdSize(float width, float height) {
    bannerAd_->setSize(width, height);
}

Task<AdResult> Self::showAppOpenAd() {
    if (!initialized_) {
        co_return AdResult::NotInitialized;
    }
    if (interstitialAd_ == nullptr) {
        co_return AdResult::NotConfigured;
    }
    auto result = co_await appOpenAd_->show();
    co_return result;
}

Task<AdResult> Self::showInterstitialAd() {
    if (!initialized_) {
        co_return AdResult::NotInitialized;
    }
    if (interstitialAd_ == nullptr) {
        co_return AdResult::NotConfigured;
    }
    auto result = co_await interstitialAd_->show();
    co_return result;
}

Task<AdResult> Self::showRewardedAd() {
    if (!initialized_) {
        co_return AdResult::NotInitialized;
    }
    if (rewardedAd_ == nullptr) {
        co_return AdResult::NotConfigured;
    }
    auto result = co_await rewardedAd_->show();
    co_return result;
}
} // namespace services
} // namespace ee
