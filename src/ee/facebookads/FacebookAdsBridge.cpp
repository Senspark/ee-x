//
//  FacebookAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/facebookads/FacebookAdsBridge.hpp"
#include "ee/ads/NullAdView.hpp"
#include "ee/ads/NullInterstitialAd.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/facebookads/FacebookNativeAdLayout.hpp"
#include "ee/facebookads/internal/FacebookBannerAd.hpp"
#include "ee/facebookads/internal/FacebookInterstitialAd.hpp"
#include "ee/facebookads/internal/FacebookNativeAd.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = FacebookAds;

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
// clang-format on
} // namespace

namespace {
// clang-format off
constexpr auto k__ad_id       = "ad_id";
constexpr auto k__ad_size     = "ad_size";
constexpr auto k__layout_name = "layout_name";
constexpr auto k__identifiers = "identifiers";
// clang-format on
} // namespace

Self::FacebookAds() {}

Self::~FacebookAds() {}

std::string Self::getTestDeviceHash() const {
    auto&& bridge = MessageBridge::getInstance();
    return bridge.call(k__getTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__addTestDevice, hash);
}

void Self::clearTestDevices() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__clearTestDevices);
}

std::shared_ptr<IAdView> Self::createBannerAd(const std::string& adId,
                                                      BannerAdSize adSize) {
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__ad_size] = static_cast<int>(adSize);

    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__createBannerAd, json.dump());
    if (not core::toBool(response)) {
        return std::make_shared<NullAdView>();
    }
    return std::shared_ptr<IAdView>(new BannerAd(this, adId));
}

bool Self::destroyBannerAd(const std::string& adId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__destroyBannerAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IAdView>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__layout_name] = layoutName;
    json[k__identifiers] = identifiers.params_;

    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__createNativeAd, json.dump());
    if (not core::toBool(response)) {
        return std::make_shared<NullAdView>();
    }
    return std::shared_ptr<IAdView>(new NativeAd(this, adId));
}

bool Self::destroyNativeAd(const std::string& adId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__destroyNativeAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& placementId) {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__createInterstitialAd, placementId);
    if (not core::toBool(response)) {
        return std::make_shared<NullInterstitialAd>();
    }
    return std::shared_ptr<IInterstitialAd>(
        new InterstitialAd(this, placementId));
}

bool Self::destroyInterstitialAd(const std::string& placementId) {
    auto&& bridge = MessageBridge::getInstance();
    auto&& response = bridge.call(k__destroyInterstitialAd, placementId);
    return core::toBool(response);
}
} // namespace facebook
} // namespace ee
