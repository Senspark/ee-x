//
//  AdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/ad_mob/private/AdMobBridge.hpp"

#include <ee/ads/internal/GuardedAdView.hpp>
#include <ee/ads/internal/GuardedInterstitialAd.hpp>
#include <ee/ads/internal/GuardedRewardedAd.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/SwitchToLibraryThread.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/ad_mob/AdMobNativeAdLayout.hpp"
#include "ee/ad_mob/private/AdMobAppOpenAd.hpp"
#include "ee/ad_mob/private/AdMobBannerAd.hpp"
#include "ee/ad_mob/private/AdMobInterstitialAd.hpp"
#include "ee/ad_mob/private/AdMobNativeAd.hpp"
#include "ee/ad_mob/private/AdMobRewardedAd.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IAdMob>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    if (not addPlugin(Plugin::AdMob)) {
        return nullptr;
    }
    return std::make_shared<admob::Bridge>(bridge);
}
} // namespace core

namespace admob {
namespace {
// clang-format off
const std::string kPrefix             = "AdMobBridge";
const auto kInitialize                = kPrefix + "Initialize";
const auto kGetEmulatorTestDeviceHash = kPrefix + "GetEmulatorTestDeviceHash";
const auto kAddTestDevice             = kPrefix + "AddTestDevice";
const auto kGetBannerAdSize           = kPrefix + "GetBannerAdSize";
const auto kCreateBannerAd            = kPrefix + "CreateBannerAd";
const auto kDestroyBannerAd           = kPrefix + "DestroyBannerAd";
const auto kCreateNativeAd            = kPrefix + "CreateNativeAd";
const auto kDestroyNativeAd           = kPrefix + "DestroyNativeAd";
const auto kCreateInterstitialAd      = kPrefix + "CreateInterstitialAd";
const auto kDestroyInterstitialAd     = kPrefix + "DestroyInterstitialAd";
const auto kCreateRewardedAd          = kPrefix + "CreateRewardedAd";
const auto kDestroyRewardedAd         = kPrefix + "DestroyRewardedAd";
const auto kCreateAppOpenAd           = kPrefix + "CreateAppOpenAd";
const auto kDestroyAppOpenAd          = kPrefix + "DestroyAppOpenAd";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge)
    , logger_(Logger::getSystemLogger()) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    interstitialAdDisplayer_ = mediation.getInterstitialAdDisplayer();
    rewardedAdDisplayer_ = mediation.getRewardedAdDisplayer();
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    for (auto&& [key, value] : bannerAds_) {
        value->destroy();
    }
    for (auto&& [key, value] : nativeAds_) {
        value->destroy();
    }
    for (auto&& [key, value] : interstitialAds_) {
        value->destroy();
    }
    for (auto&& [key, value] : rewardedAds_) {
        value->destroy();
    }
    for (auto&& [key, value] : appOpenAds_) {
        value->destroy();
    }
    bannerAds_.clear();
    nativeAds_.clear();
    interstitialAds_.clear();
    rewardedAds_.clear();
    appOpenAds_.clear();
    PluginManager::removePlugin(Plugin::AdMob);
}

Task<bool> Self::initialize() {
    auto response = co_await bridge_.callAsync(kInitialize);
    co_await SwitchToLibraryThread();
    co_return core::toBool(response);
}

std::string Self::getEmulatorTestDeviceHash() const {
    return bridge_.call(kGetEmulatorTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    bridge_.call(kAddTestDevice, hash);
}

std::pair<int, int> Self::getBannerAdSize(BannerAdSize adSize) {
    auto response = bridge_.call(kGetBannerAdSize,
                                 std::to_string(static_cast<int>(adSize)));
    auto json = nlohmann::json::parse(response);
    int width = json["width"];
    int height = json["height"];
    return std::pair(width, height);
}

std::shared_ptr<IAdView> Self::createBannerAd(const std::string& adId,
                                              BannerAdSize adSize) {
    logger_.debug("%s: id = %s size = %d", __PRETTY_FUNCTION__, adId.c_str(),
                  static_cast<int>(adSize));
    auto iter = bannerAds_.find(adId);
    if (iter != bannerAds_.cend()) {
        return iter->second;
    }
    nlohmann::json json;
    json["adId"] = adId;
    json["adSize"] = static_cast<int>(adSize);
    auto response = bridge_.call(kCreateBannerAd, json.dump());
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto size = getBannerAdSize(adSize);
    auto ad = std::make_shared<ads::GuardedAdView>(std::shared_ptr<IAdView>(
        new BannerAd(bridge_, logger_, this, adId, size)));
    bannerAds_.emplace(adId, ad);
    return ad;
}

bool Self::destroyBannerAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = bannerAds_.find(adId);
    if (iter == bannerAds_.cend()) {
        return false;
    }
    auto response = bridge_.call(kDestroyBannerAd, adId);
    if (core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    bannerAds_.erase(iter);
    return true;
}

std::shared_ptr<IAdView>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = nativeAds_.find(adId);
    if (iter != nativeAds_.cend()) {
        return iter->second;
    }
    nlohmann::json json;
    json["adId"] = adId;
    json["layoutName"] = layoutName;
    json["identifiers"] = identifiers.params_;
    auto&& response = bridge_.call(kCreateNativeAd, json.dump());
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto ad = std::make_shared<ads::GuardedAdView>(
        std::shared_ptr<IAdView>(new NativeAd(bridge_, logger_, this, adId)));
    nativeAds_.emplace(adId, ad);
    return ad;
}

bool Self::destroyNativeAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = nativeAds_.find(adId);
    if (iter == nativeAds_.cend()) {
        return false;
    }
    auto&& response = bridge_.call(kDestroyNativeAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    nativeAds_.erase(iter);
    return true;
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = interstitialAds_.find(adId);
    if (iter != interstitialAds_.cend()) {
        return iter->second;
    }
    auto response = bridge_.call(kCreateInterstitialAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto ad = std::make_shared<ads::GuardedInterstitialAd>(
        std::shared_ptr<IInterstitialAd>(new InterstitialAd(
            bridge_, logger_, interstitialAdDisplayer_, this, adId)));
    interstitialAds_.emplace(adId, ad);
    return ad;
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = interstitialAds_.find(adId);
    if (iter == interstitialAds_.cend()) {
        return false;
    }
    auto&& response = bridge_.call(kDestroyInterstitialAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    interstitialAds_.erase(iter);
    return true;
}

std::shared_ptr<IRewardedAd> Self::createRewardedAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        return iter->second;
    }
    auto response = bridge_.call(kCreateRewardedAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto ad = std::make_shared<ads::GuardedRewardedAd>(
        std::shared_ptr<IRewardedAd>(new RewardedAd(
            bridge_, logger_, rewardedAdDisplayer_, this, adId)));
    rewardedAds_.emplace(adId, ad);
    return ad;
}

bool Self::destroyRewardedAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = rewardedAds_.find(adId);
    if (iter == rewardedAds_.cend()) {
        return false;
    }
    auto&& response = bridge_.call(kDestroyRewardedAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    rewardedAds_.erase(iter);
    return true;
}

std::shared_ptr<IInterstitialAd>
Self::createAppOpenAd(const std::string& adId,
                      AppOpenAdOrientation orientation) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = appOpenAds_.find(adId);
    if (iter != appOpenAds_.cend()) {
        return iter->second;
    }
    auto response = bridge_.call(kCreateAppOpenAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto ad = std::make_shared<ads::GuardedInterstitialAd>(
        std::shared_ptr<IInterstitialAd>(
            new AppOpenAd(bridge_, logger_, interstitialAdDisplayer_, this,
                          adId, orientation)));
    appOpenAds_.emplace(adId, ad);
    return ad;
}

bool Self::destroyAppOpenAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = appOpenAds_.find(adId);
    if (iter == appOpenAds_.cend()) {
        return false;
    }
    auto&& response = bridge_.call(kDestroyAppOpenAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    appOpenAds_.erase(iter);
    return true;
}
} // namespace admob
} // namespace ee
