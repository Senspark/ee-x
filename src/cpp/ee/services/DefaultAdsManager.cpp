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
    initializeFullScreenAd(AdFormat::AppOpen);
    initializeFullScreenAd(AdFormat::Interstitial);
    initializeFullScreenAd(AdFormat::Rewarded);
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
    return bannerAd_->isLoaded();
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
    co_return co_await showFullScreenAd(AdFormat::AppOpen);
}

Task<AdResult> Self::showInterstitialAd() {
    co_return co_await showFullScreenAd(AdFormat::Interstitial);
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
    auto result = co_await ad->show();
    co_return result;
}
} // namespace services
} // namespace ee
