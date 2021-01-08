//
//  FacebookAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/facebook_ads/private/FacebookAdsBridge.hpp"

#include <ee/ads/internal/GuardedAdView.hpp>
#include <ee/ads/internal/GuardedFullScreenAd.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/LogLevel.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/SwitchToLibraryThread.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/facebook_ads/FacebookNativeAdLayout.hpp"
#include "ee/facebook_ads/private/FacebookBannerAd.hpp"
#include "ee/facebook_ads/private/FacebookInterstitialAd.hpp"
#include "ee/facebook_ads/private/FacebookNativeAd.hpp"
#include "ee/facebook_ads/private/FacebookRewardedAd.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IFacebookAds>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    addPlugin(Plugin::FacebookAds);
    return std::make_shared<facebook_ads::Bridge>(bridge);
}
} // namespace core

namespace facebook_ads {
namespace {
// clang-format off
const std::string kPrefix         = "FacebookAdsBridge";

const auto kInitialize            = kPrefix + "Initialize";

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
constexpr auto k__ad_id       = "adId";
constexpr auto k__ad_size     = "adSize";
constexpr auto k__layout_name = "layoutName";
constexpr auto k__identifiers = "identifiers";
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
    for (auto&& [key, value] : ads_) {
        value->destroy();
    }
    PluginManager::removePlugin(Plugin::FacebookAds);
}

Task<bool> Self::initialize() {
    auto response = co_await bridge_.callAsync(kInitialize);
    co_await SwitchToLibraryThread();
    co_return core::toBool(response);
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
    logger_.debug("%s: id = %s size = %d", __PRETTY_FUNCTION__, adId.c_str(),
                  static_cast<int>(adSize));
    auto iter = ads_.find(adId);
    if (iter != ads_.cend()) {
        return std::dynamic_pointer_cast<IAdView>(iter->second);
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
    auto ad = std::make_shared<ads::GuardedAdView>(
        std::make_shared<BannerAd>(bridge_, logger_, this, adId, size));
    ads_.emplace(adId, ad);
    return ad;
}

std::shared_ptr<IAdView>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter != ads_.cend()) {
        return std::dynamic_pointer_cast<IAdView>(iter->second);
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
        std::make_shared<NativeAd>(bridge_, logger_, this, adId));
    ads_.emplace(adId, ad);
    return ad;
}

std::shared_ptr<IFullScreenAd>
Self::createInterstitialAd(const std::string& adId) {
    return createFullScreenAd<InterstitialAd>(kCreateInterstitialAd, adId);
}

std::shared_ptr<IFullScreenAd> Self::createRewardedAd(const std::string& adId) {
    return createFullScreenAd<RewardedAd>(kCreateRewardedAd, adId);
}

template <class Ad>
std::shared_ptr<IFullScreenAd>
Self::createFullScreenAd(const std::string& handlerId,
                         const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter != ads_.cend()) {
        return std::dynamic_pointer_cast<IFullScreenAd>(iter->second);
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
    ads_.emplace(adId, ad);
    return ad;
}

bool Self::destroyBannerAd(const std::string& adId) {
    return destroyAd(kDestroyBannerAd, adId);
}

bool Self::destroyNativeAd(const std::string& adId) {
    return destroyAd(kDestroyNativeAd, adId);
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    return destroyAd(kDestroyInterstitialAd, adId);
}

bool Self::destroyRewardedAd(const std::string& adId) {
    return destroyAd(kDestroyRewardedAd, adId);
}

bool Self::destroyAd(const std::string& handlerId, const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter == ads_.cend()) {
        return false;
    }
    auto&& response = bridge_.call(handlerId, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    ads_.erase(iter);
    return true;
}
} // namespace facebook_ads
} // namespace ee
