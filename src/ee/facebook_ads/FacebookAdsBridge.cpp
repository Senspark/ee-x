//
//  FacebookAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/facebook_ads/FacebookAdsBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/ads/internal/GuardedAdView.hpp>
#include <ee/ads/internal/GuardedInterstitialAd.hpp>
#include <ee/ads/internal/GuardedRewardedAd.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/LogLevel.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/facebook_ads/FacebookNativeAdLayout.hpp"
#include "ee/facebook_ads/private/FacebookBannerAd.hpp"
#include "ee/facebook_ads/private/FacebookInterstitialAd.hpp"
#include "ee/facebook_ads/private/FacebookNativeAd.hpp"
#include "ee/facebook_ads/private/FacebookRewardedAd.hpp"

namespace ee {
namespace facebook_ads {
using Self = Bridge;

namespace {
// clang-format off
const std::string kPrefix         = "FacebookAdsBridge";

const auto kGetTestDeviceHash     = kPrefix + "GetTestDeviceHash";
const auto kAddTestDevice         = kPrefix + "AddTestDevice";
const auto kClearTestDevices      = kPrefix + "ClearTestDevices";

const auto kGetBannerAdSize       = kPrefix + "GetBannerAdSize";
const auto kCreateBannerAd        = kPrefix + "CreateBannerAd";
const auto kDestroyBannerAd       = kPrefix + "DestroyBannerAd";

const auto kCreateNativeAd        = kPrefix + "CreateNativeAd";
const auto kDestroyNativeAd       = kPrefix + "DestroyNativeAd";

const auto kCreateInterstitialAd  = kPrefix + "CreateInterstitialAd";
const auto kDestroyInterstitialAd = kPrefix + "DestroyInterstitialAd";
    
const auto kCreateRewardedAd      = kPrefix + "CreateRewardedAd";
const auto kDestroyRewardedAd     = kPrefix + "DestroyRewardedAd";
} // namespace

namespace {
// clang-format off
constexpr auto k__ad_id       = "ad_id";
constexpr auto k__ad_size     = "ad_size";
constexpr auto k__layout_name = "layout_name";
constexpr auto k__identifiers = "identifiers";
// clang-format on
} // namespace

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    PluginManager::addPlugin(Plugin::FacebookAds);

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
    PluginManager::removePlugin(Plugin::FacebookAds);
}

std::string Self::getTestDeviceHash() const {
    return bridge_.call(kGetTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    bridge_.call(kAddTestDevice, hash);
}

void Self::clearTestDevices() {
    bridge_.call(kClearTestDevices);
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
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = bannerAds_.find(adId);
    if (iter != bannerAds_.cend()) {
        return iter->second;
    }
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__ad_size] = static_cast<int>(adSize);
    auto response = bridge_.call(kCreateBannerAd, json.dump());
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad",
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
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad",
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
    json[k__ad_id] = adId;
    json[k__layout_name] = layoutName;
    json[k__identifiers] = identifiers.params_;
    auto response = bridge_.call(kCreateNativeAd, json.dump());
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad",
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
    auto response = bridge_.call(kDestroyNativeAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad",
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
        logger_.error("%s: There was an error when attempt to create an ad",
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
        logger_.error("%s: There was an error when attempt to destroy an ad",
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
        logger_.error("%s: There was an error when attempt to create an ad",
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
        logger_.error("%s: There was an error when attempt to destroy an ad",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    rewardedAds_.erase(iter);
    return true;
}
} // namespace facebook_ads
} // namespace ee
