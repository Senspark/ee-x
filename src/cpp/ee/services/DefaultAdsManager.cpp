#include "ee/services/DefaultAdsManager.hpp"

#include <cassert>

#include <ee/cocos/CocosAdView.hpp>
#include <ee/core/NoAwait.hpp>
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
}

void Self::initializeAppOpenAd() {
    appOpenAd_ = std::dynamic_pointer_cast<GenericAd>(
        config_->createAd(AdFormat::AppOpen));
}

void Self::initializeInterstitialAd() {
    interstitialAd_ = std::dynamic_pointer_cast<GenericAd>(
        config_->createAd(AdFormat::Interstitial));
}

void Self::initializeRewardedAd() {
    rewardedAd_ = std::dynamic_pointer_cast<GenericAd>(
        config_->createAd(AdFormat::Rewarded));
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
