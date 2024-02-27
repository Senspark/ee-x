#include "ee/services/DefaultAdsManager.hpp"

#include <cassert>

#include <ee/ads/AdFormat.hpp>
#include <ee/ads/AdNetwork.hpp>
#include <ee/ads/internal/Capper.hpp>
#include <ee/core/Delay.hpp>
#include <ee/core/NoAwait.hpp>
#include <ee/core/ObserverHandle.hpp>
#include <ee/core/Task.hpp>

#include "ee/services/internal/AdsConfig.hpp"
#include "ee/services/internal/LazyBannerAd.hpp"
#include "ee/services/internal/LazyFullScreenAd.hpp"

namespace ee {
namespace services {
using Self = DefaultAdsManager;

Self::DefaultAdsManager(const std::string& configJson) {
    initialized_ = false;
    config_ = AdsConfig::parse(configJson);
    auto displayCapper = std::make_shared<ads::Capper>(0.1f);
    bannerAds_ = {
        {AdFormat::Banner, std::make_shared<LazyBannerAd>()},
        {AdFormat::Rectangle, std::make_shared<LazyBannerAd>()},
    };
    fullScreenAds_ = {
        {AdFormat::AppOpen, std::make_shared<LazyFullScreenAd>(displayCapper)},
        {AdFormat::Interstitial,
         std::make_shared<LazyFullScreenAd>(displayCapper)},
        {AdFormat::RewardedInterstitial,
         std::make_shared<LazyFullScreenAd>(displayCapper)},
        {AdFormat::Rewarded, std::make_shared<LazyFullScreenAd>(displayCapper)},
    };
}

Task<bool> Self::initialize() {
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
}

void Self::initializeFullScreenAd(AdFormat format) {
    auto ad =
        std::dynamic_pointer_cast<IFullScreenAd>(config_->createAd(format));
    if (ad == nullptr) {
        return;
    }
    fullScreenAds_.at(format)->setAd(ad);
}

void Self::addTestDevice(const std::string& hash) {
    config_->addTestDevice(hash);
}

void Self::openTestSuite() {
    config_->openTestSuite(AdNetwork::AdMob);
}

void Self::addAnalytics(std::shared_ptr<ILibraryAnalytics> analytics) {
    config_->addAnalytics(analytics);
}

std::shared_ptr<IBannerAd> Self::getBannerAd() {
    return bannerAds_.at(AdFormat::Banner);
}

std::shared_ptr<IBannerAd> Self::getRectangleAd() {
    return bannerAds_.at(AdFormat::Rectangle);
}

std::shared_ptr<IFullScreenAd> Self::getAppOpenAd() {
    return fullScreenAds_.at(AdFormat::AppOpen);
}

std::shared_ptr<IFullScreenAd> Self::getInterstitialAd() {
    return fullScreenAds_.at(AdFormat::Interstitial);
}

std::shared_ptr<IFullScreenAd> Self::getRewardedInterstitialAd() {
    return fullScreenAds_.at(AdFormat::RewardedInterstitial);
}

std::shared_ptr<IFullScreenAd> Self::getRewardedAd() {
    return fullScreenAds_.at(AdFormat::Rewarded);
}
} // namespace services
} // namespace ee
