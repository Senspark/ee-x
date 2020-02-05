//
//  FacebookAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/facebook_ads/FacebookAdsBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/ads/NullAdView.hpp>
#include <ee/ads/NullInterstitialAd.hpp>
#include <ee/ads/NullRewardedVideo.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/LogLevel.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/core/internal/SharedPtrUtils.hpp>

#include "ee/facebook_ads/FacebookNativeAdLayout.hpp"
#include "ee/facebook_ads/private/FacebookBannerAd.hpp"
#include "ee/facebook_ads/private/FacebookInterstitialAd.hpp"
#include "ee/facebook_ads/private/FacebookNativeAd.hpp"
#include "ee/facebook_ads/private/FacebookRewardVideoAd.hpp"

namespace ee {
namespace facebook_ads {
using Self = Bridge;

namespace {
// clang-format off
constexpr auto k__getTestDeviceHash     = "FacebookAds_getTestDeviceHash";
constexpr auto k__addTestDevice         = "FacebookAds_addTestDevice";
constexpr auto k__clearTestDevices      = "FacebookAds_clearTestDevices";

constexpr auto k__createBannerAd        = "FacebookAds_createBannerAd";
constexpr auto k__destroyBannerAd       = "FacebookAds_destroyBannerAd";

constexpr auto k__createNativeAd        = "FacebookAds_createNativeAd";
constexpr auto k__destroyNativeAd       = "FacebookAds_destroyNativeAd";

constexpr auto k__createInterstitialAd  = "FacebookAds_createInterstitialAd";
constexpr auto k__destroyInterstitialAd = "FacebookAds_destroyInterstitialAd";
    
constexpr auto k__createRewardVideoAd   = "FacebookAds_createRewardVideoAd";
constexpr auto k__destroyRewardVideoAd  = "FacebookAds_destroyRewardVideoAd";
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
    , logger_(logger) {}

Self::~Bridge() {}

std::string Self::getTestDeviceHash() const {
    return bridge_.call(k__getTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    bridge_.call(k__addTestDevice, hash);
}

void Self::clearTestDevices() {
    bridge_.call(k__clearTestDevices);
}

std::shared_ptr<IAdView> Self::createBannerAd(const std::string& adId,
                                              BannerAdSize adSize) {
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__ad_size] = static_cast<int>(adSize);
    auto response = bridge_.call(k__createBannerAd, json.dump());
    if (not core::toBool(response)) {
        return core::makeShared<NullAdView>();
    }
    return std::shared_ptr<IAdView>(new BannerAd(bridge_, this, adId));
}

bool Self::destroyBannerAd(const std::string& adId) {
    auto response = bridge_.call(k__destroyBannerAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IAdView>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__layout_name] = layoutName;
    json[k__identifiers] = identifiers.params_;
    auto response = bridge_.call(k__createNativeAd, json.dump());
    if (not core::toBool(response)) {
        return core::makeShared<NullAdView>();
    }
    return std::shared_ptr<IAdView>(new NativeAd(bridge_, logger_, this, adId));
}

bool Self::destroyNativeAd(const std::string& adId) {
    auto response = bridge_.call(k__destroyNativeAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& placementId) {
    auto response = bridge_.call(k__createInterstitialAd, placementId);
    if (not core::toBool(response)) {
        return core::makeShared<NullInterstitialAd>();
    }
    return std::shared_ptr<IInterstitialAd>(
        new InterstitialAd(bridge_, logger_, this, placementId));
}

bool Self::destroyInterstitialAd(const std::string& placementId) {
    auto&& response = bridge_.call(k__destroyInterstitialAd, placementId);
    return core::toBool(response);
}

std::shared_ptr<IRewardedVideo>
Self::createRewardedVideo(const std::string& placementId) {
    auto response = bridge_.call(k__createRewardVideoAd, placementId);
    if (not core::toBool(response)) {
        return core::makeShared<NullRewardedVideo>(logger_);
    }
    return std::shared_ptr<IRewardedVideo>(
        new RewardedVideo(bridge_, logger_, placementId));
}

bool Self::destroyRewardVideoAd(const std::string& placementId) {
    auto&& response = bridge_.call(k__destroyRewardVideoAd, placementId);
    return core::toBool(response);
}
} // namespace facebook_ads
} // namespace ee
