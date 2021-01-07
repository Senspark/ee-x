#include "ee/services/DefaultAdsManager.hpp"

#include <cassert>

#include <ee/ad_mob/AdMobBannerAdSize.hpp>
#include <ee/ad_mob/IAdMobBridge.hpp>
#include <ee/ads/MultiAdView.hpp>
#include <ee/ads/MultiFullScreenAd.hpp>
#include <ee/app_lovin/IAppLovinBridge.hpp>
#include <ee/cocos/CocosAdView.hpp>
#include <ee/core/Delay.hpp>
#include <ee/core/NoAwait.hpp>
#include <ee/core/Platform.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/WhenAll.hpp>
#include <ee/facebook_ads/FacebookBannerAdSize.hpp>
#include <ee/facebook_ads/IFacebookAdsBridge.hpp>
#include <ee/iron_source/IIronSourceBridge.hpp>
#include <ee/unity_ads/IUnityAdsBridge.hpp>
#include <ee/vungle/IVungleBridge.hpp>

#include "ee/services/internal/AdsConfig.hpp"

namespace ee {
namespace services {
using Self = DefaultAdsManager;

Self::DefaultAdsManager(const std::string& configJson) {
    config_ = AdsConfig::parse(configJson);
}

Task<bool> Self::initialize() {
    co_await config_->initialize();
    initialized_ = true;
    co_return true;
}

void Self::initializeBannerAd() {
    auto ad = std::dynamic_pointer_cast<IAdView>(config_->createAd(AdFormat::Banner));
    if (ad == nullptr) {
        return;
    }
    bannerAd_ = std::make_shared<CocosAdView>(ad);
    bannerAd_->setVisible(isBannerAdVisible_);
    bannerAd_->setAnchor(bannerAdAnchor_.first, bannerAdAnchor_.second);
    bannerAd_->setPosition(bannerAdPosition_.first, bannerAdPosition_.second);
    if (bannerAdSize_.has_value()) {
        bannerAd_->setSize(bannerAdSize_->first, bannerAdSize_->second);
    }
    noAwait([this]() -> Task<> {
        auto result = co_await bannerAd_->load();
        if (result && bannerAd_->isVisible()) {
            bannerAd_->setVisible(false);
            bannerAd_->setVisible(true);
        }
    });
}

void Self::initializeAppOpenAd() {
    auto ad = std::dynamic_pointer_cast<IFullScreenAd>(config_->createAd(AdFormat::AppOpen));
    if (ad == nullptr) {
        return;
    }
    appOpenAd_ = ad;
    noAwait(appOpenAd_->load());
//    appOpenAdInterval_ = config->interval();
    isAppOpenAdCapped_ = true;
    noAwait([this]() -> Task<> {
        co_await Delay(appOpenAdInterval_);
        isAppOpenAdCapped_ = false;
    });
}

void Self::createAd(const std::shared_ptr<InterstitialConfig>& config) {
    if (interstitialAd_ != nullptr) {
        return;
    }
//    interstitialAd_ = createInterstitialAd(config->instance());
    noAwait(interstitialAd_->load());
    interstitialAdInterval_ = config->interval();
    isInterstitialAdCapped_ = true;
    noAwait([this]() -> Task<> {
        co_await Delay(interstitialAdInterval_);
        isInterstitialAdCapped_ = false;
    });
}

void Self::createAd(const std::shared_ptr<RewardedConfig>& config) {
    if (rewardedAd_ != nullptr) {
        return;
    }
//    rewardedAd_ = createRewardedAd(config->instance());
    noAwait(rewardedAd_->load());
}

bool Self::isBannerAdVisible() const {
    return isBannerAdVisible_;
}

void Self::setBannerAdVisible(bool visible) {
    isBannerAdVisible_ = visible;
    if (bannerAd_ != nullptr) {
        bannerAd_->setVisible(visible);
    }
}

std::pair<float, float> Self::getBannerAdAnchor() const {
    return bannerAdAnchor_;
}

void Self::setBannerAdAnchor(float x, float y) {
    bannerAdAnchor_ = std::pair(x, y);
    if (bannerAd_ != nullptr) {
        bannerAd_->setAnchor(x, y);
    }
}

std::pair<float, float> Self::getBannerAdPosition() const {
    return bannerAdPosition_;
}

void Self::setBannerAdPosition(float x, float y) {
    bannerAdPosition_ = std::pair(x, y);
    if (bannerAd_ != nullptr) {
        bannerAd_->setPosition(x, y);
    }
}

std::pair<float, float> Self::getBannerAdSize() const {
    if (bannerAd_ != nullptr) {
        return bannerAd_->getSize();
    }
    return bannerAdSize_.value_or(std::pair(0, 0));
}

void Self::setBannerAdSize(float width, float height) {
    bannerAdSize_ = std::pair(width, height);
    if (bannerAd_ != nullptr) {
        bannerAd_->setSize(width, height);
    }
}

Task<AdResult> Self::showInterstitialAd() {
    if (!initialized_) {
        co_return AdResult::NotInitialized;
    }
    if (interstitialAd_ == nullptr) {
        co_return AdResult::NotConfigured;
    }
    if (interstitialAdInterval_ > 0 && isInterstitialAdCapped_) {
        co_return AdResult::Capped;
    }
    if (interstitialAd_->isLoaded()) {
        auto hasInternet = co_await testConnection(0.2f);
        if (!hasInternet) {
            co_return AdResult::NoInternet;
        }
    } else {
        noAwait(interstitialAd_->load());
        auto hasInternet = false;
        co_await whenAll(    //
            []() -> Task<> { //
                co_await Delay(0.1f);
            },
            [&hasInternet]() -> Task<> {
                hasInternet = co_await testConnection(0.2f);
            });
        if (!hasInternet) {
            co_return AdResult::NoInternet;
        }
    }
    if (!interstitialAd_->isLoaded()) {
        co_return AdResult::NotLoaded;
    }
    auto result = co_await interstitialAd_->show();
    noAwait(interstitialAd_->load());
    switch (result) {
    case FullScreenAdResult::Completed:
        if (interstitialAdInterval_ > 0) {
            isInterstitialAdCapped_ = true;
            noAwait([this]() -> Task<> {
                co_await Delay(interstitialAdInterval_);
                isInterstitialAdCapped_ = false;
            });
        }
        co_return AdResult::Completed;
    default:
        co_return AdResult::Failed;
    }
}

Task<AdResult> Self::showRewardedAd() {
    if (!initialized_) {
        co_return AdResult::NotInitialized;
    }
    if (rewardedAd_ == nullptr) {
        co_return AdResult::NotConfigured;
    }
    if (rewardedAd_->isLoaded()) {
        auto hasInternet = co_await testConnection(0.2f);
        if (!hasInternet) {
            co_return AdResult::NoInternet;
        }
    } else {
        noAwait(rewardedAd_->load());
        auto hasInternet = false;
        co_await whenAll(    //
            []() -> Task<> { //
                co_await Delay(0.1f);
            },
            [&hasInternet]() -> Task<> {
                hasInternet = co_await testConnection(0.2f);
            });
        if (!hasInternet) {
            co_return AdResult::NoInternet;
        }
    }
    if (!rewardedAd_->isLoaded()) {
        co_return AdResult::NotLoaded;
    }
    auto result = co_await rewardedAd_->show();
    noAwait(rewardedAd_->load());
    switch (result) {
    case IRewardedAdResult::Failed:
        co_return AdResult::Failed;
    case IRewardedAdResult::Canceled:
        co_return AdResult::Canceled;
    case IRewardedAdResult::Completed:
        co_return AdResult::Completed;
    default:
        assert(false);
        co_return AdResult::Failed;
    }
}

Task<bool> Self::testConnection(float timeOut) {
    co_return co_await Platform::testConnection("www.google.com", timeOut);
}
} // namespace services
} // namespace ee
