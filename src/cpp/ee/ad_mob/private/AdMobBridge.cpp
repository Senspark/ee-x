//
//  AdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/ad_mob/private/AdMobBridge.hpp"

#include <ee/ads/internal/GuardedAdView.hpp>
#include <ee/ads/internal/GuardedFullScreenAd.hpp>
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
const auto kCreateAppOpenAd           = kPrefix + "CreateAppOpenAd";
const auto kDestroyAppOpenAd          = kPrefix + "DestroyAppOpenAd";
const auto kCreateInterstitialAd      = kPrefix + "CreateInterstitialAd";
const auto kDestroyInterstitialAd     = kPrefix + "DestroyInterstitialAd";
const auto kCreateRewardedAd          = kPrefix + "CreateRewardedAd";
const auto kDestroyRewardedAd         = kPrefix + "DestroyRewardedAd";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge)
    , logger_(Logger::getSystemLogger()) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    displayer_ = mediation.getAdDisplayer();
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
    for (auto&& [key, value] : fullScreenAds_) {
        value->destroy();
    }
    bannerAds_.clear();
    nativeAds_.clear();
    fullScreenAds_.clear();
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
    auto ad = std::make_shared<ads::GuardedAdView>(
        std::make_shared<BannerAd>(bridge_, logger_, this, adId, size));
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
        std::make_shared<NativeAd>(bridge_, logger_, this, adId));
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

std::shared_ptr<IFullScreenAd> Self::createAppOpenAd(const std::string& adId) {
    return createFullScreenAd<AppOpenAd>(adId, kCreateAppOpenAd);
}

bool Self::destroyAppOpenAd(const std::string& adId) {
    return destroyFullScreenAd(adId, kDestroyAppOpenAd);
}

std::shared_ptr<IFullScreenAd>
Self::createInterstitialAd(const std::string& adId) {
    return createFullScreenAd<InterstitialAd>(adId, kCreateInterstitialAd);
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    return destroyFullScreenAd(adId, kDestroyInterstitialAd);
}

std::shared_ptr<IFullScreenAd> Self::createRewardedAd(const std::string& adId) {
    return createFullScreenAd<RewardedAd>(adId, kCreateAppOpenAd);
}

bool Self::destroyRewardedAd(const std::string& adId) {
    return destroyFullScreenAd(adId, kDestroyRewardedAd);
}

template <class Ad>
std::shared_ptr<IFullScreenAd>
Self::createFullScreenAd(const std::string& adId,
                         const std::string& handlerId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = fullScreenAds_.find(adId);
    if (iter != fullScreenAds_.cend()) {
        return iter->second;
    }
    auto response = bridge_.call(handlerId, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto ad = std::make_shared<ads::GuardedFullScreenAd>(
        std::make_shared<Ad>(bridge_, logger_, displayer_, this, adId));
    fullScreenAds_.emplace(adId, ad);
    return ad;
}

bool Self::destroyFullScreenAd(const std::string& adId,
                               const std::string& handlerId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = fullScreenAds_.find(adId);
    if (iter == fullScreenAds_.cend()) {
        return false;
    }
    auto&& response = bridge_.call(handlerId, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    fullScreenAds_.erase(iter);
    return true;
}
} // namespace admob
} // namespace ee
