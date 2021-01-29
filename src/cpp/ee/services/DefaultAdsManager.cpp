#include "ee/services/DefaultAdsManager.hpp"

#include <cassert>

#include <ee/cocos/CocosBannerAd.hpp>
#include <ee/core/Delay.hpp>
#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

#include "ee/services/AdResult.hpp"
#include "ee/services/internal/AdsConfig.hpp"
#include "ee/services/internal/GenericAd.hpp"
#include "ee/services/internal/LazyBannerAd.hpp"

namespace ee {
namespace services {
using Self = DefaultAdsManager;

Self::DefaultAdsManager(const std::string& configJson) {
    initialized_ = false;
    fullScreenAdCapped_ = false;
    config_ = AdsConfig::parse(configJson);
    handle_ = std::make_unique<ObserverHandle>();
}

Task<bool> Self::initialize() {
    bannerAds_[AdFormat::Banner] = std::make_shared<LazyBannerAd>();
    bannerAds_[AdFormat::Rectangle] = std::make_shared<LazyBannerAd>();
    co_await config_->initialize();
    initializeBannerAd(AdFormat::Banner);
    initializeBannerAd(AdFormat::Rectangle);
    initializeFullScreenAd(AdFormat::AppOpen);
    initializeFullScreenAd(AdFormat::Interstitial);
    initializeFullScreenAd(AdFormat::RewardedInterstitial);
    initializeFullScreenAd(AdFormat::Rewarded);
    initialized_ = true;
    co_return true;
}

void Self::initializeBannerAd(AdFormat format) {
    auto ad = std::dynamic_pointer_cast<IBannerAd>(config_->createAd(format));
    if (ad == nullptr) {
        return;
    }
    bannerAds_.at(format)->setAd(ad);
    (*handle_) //
        .bind(*bannerAds_.at(format))
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

void Self::initializeFullScreenAd(AdFormat format) {
    auto ad = std::dynamic_pointer_cast<GenericAd>(config_->createAd(format));
    if (ad == nullptr) {
        return;
    }
    fullScreenAds_[format] = ad;
    (*handle_) //
        .bind(*ad)
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

bool Self::isBannerAdLoaded() const {
    return bannerAds_.at(AdFormat::Banner)->isLoaded();
}

bool Self::isBannerAdVisible() const {
    return bannerAds_.at(AdFormat::Banner)->isVisible();
}

void Self::setBannerAdVisible(bool visible) {
    bannerAds_.at(AdFormat::Banner)->setVisible(visible);
}

std::pair<float, float> Self::getBannerAdAnchor() const {
    return bannerAds_.at(AdFormat::Banner)->getAnchor();
}

void Self::setBannerAdAnchor(float x, float y) {
    bannerAds_.at(AdFormat::Banner)->setAnchor(x, y);
}

std::pair<float, float> Self::getBannerAdPosition() const {
    return bannerAds_.at(AdFormat::Banner)->getPosition();
}

void Self::setBannerAdPosition(float x, float y) {
    bannerAds_.at(AdFormat::Banner)->setPosition(x, y);
}

std::pair<float, float> Self::getBannerAdSize() const {
    return bannerAds_.at(AdFormat::Banner)->getSize();
}

void Self::setBannerAdSize(float width, float height) {
    bannerAds_.at(AdFormat::Banner)->setSize(width, height);
}

bool Self::isRectangleAdLoaded() const {
    return bannerAds_.at(AdFormat::Rectangle)->isLoaded();
}

bool Self::isRectangleAdVisible() const {
    return bannerAds_.at(AdFormat::Rectangle)->isVisible();
}

void Self::setRectangleAdVisible(bool visible) {
    bannerAds_.at(AdFormat::Rectangle)->setVisible(visible);
}

std::pair<float, float> Self::getRectangleAdAnchor() const {
    return bannerAds_.at(AdFormat::Rectangle)->getAnchor();
}

void Self::setRectangleAdAnchor(float x, float y) {
    bannerAds_.at(AdFormat::Rectangle)->setAnchor(x, y);
}

std::pair<float, float> Self::getRectangleAdPosition() const {
    return bannerAds_.at(AdFormat::Rectangle)->getPosition();
}

void Self::setRectangleAdPosition(float x, float y) {
    bannerAds_.at(AdFormat::Rectangle)->setPosition(x, y);
}

std::pair<float, float> Self::getRectangleAdSize() const {
    return bannerAds_.at(AdFormat::Rectangle)->getSize();
}

void Self::setRectangleAdSize(float width, float height) {
    bannerAds_.at(AdFormat::Rectangle)->setSize(width, height);
}

Task<AdResult> Self::showAppOpenAd() {
    co_return co_await showFullScreenAd(AdFormat::AppOpen);
}

Task<AdResult> Self::showInterstitialAd() {
    co_return co_await showFullScreenAd(AdFormat::Interstitial);
}

Task<AdResult> Self::showRewardedInterstitialAd() {
    co_return co_await showFullScreenAd(AdFormat::RewardedInterstitial);
}

Task<AdResult> Self::showRewardedAd() {
    co_return co_await showFullScreenAd(AdFormat::Rewarded);
}

Task<AdResult> Self::showFullScreenAd(AdFormat format) {
    if (!initialized_) {
        co_return AdResult::NotInitialized;
    }
    auto&& ad = fullScreenAds_[format];
    if (ad == nullptr) {
        co_return AdResult::NotConfigured;
    }
    if (fullScreenAdCapped_) {
        co_return AdResult::Capped;
    }
    fullScreenAdCapped_ = true;
    auto result = co_await ad->show();
    noAwait([this]() -> Task<> {
        co_await Delay(0.1f);
        fullScreenAdCapped_ = false;
    });
    co_return result;
}
} // namespace services
} // namespace ee
