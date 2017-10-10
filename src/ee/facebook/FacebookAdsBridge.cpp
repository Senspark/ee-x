//
//  FacebookAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/facebook/FacebookAdsBridge.hpp"
#include "ee/core/LogLevel.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/facebook/FacebookBannerAd.hpp"
#include "ee/facebook/FacebookNativeAd.hpp"
#include "ee/facebook/FacebookNativeAdBuilder.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = FacebookAds;

namespace {
// clang-format off
constexpr auto k__getTestDeviceHash   = "FacebookAds_getTestDeviceHash";
constexpr auto k__addTestDevice       = "FacebookAds_addTestDevice";
constexpr auto k__clearTestDevices    = "FacebookAds_clearTestDevices";

constexpr auto k__createBannerAd      = "FacebookAds_createBannerAd";
constexpr auto k__destroyBannerAd     = "FacebookAds_destroyBannerAd";

constexpr auto k__createNativeAd      = "FacebookAds_createNativeAd";
constexpr auto k__destroyNativeAd     = "FacebookAds_destroyNativeAd";

constexpr auto k__cacheInterstitialAd = "FacebookAds_cacheInterstitialAd";
constexpr auto k__showInterstitialAd  = "FacebookAds_showInterstitialAd";

constexpr auto k__cppCallback         = "FacebookAds_cppCallback";
// clang-format on
} // namespace

Self::FacebookAds() {}

Self::~FacebookAds() {}

std::string Self::getTestDeviceHash() const {
    auto&& bridge = core::MessageBridge::getInstance();
    return bridge.call(k__getTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__addTestDevice, hash);
}

void Self::clearTestDevices() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__clearTestDevices);
}

std::shared_ptr<AdViewInterface>
Self::createBannerAd(const std::string& adId, FacebookBannerAdSize adSize) {
    nlohmann::json json;
    json["adId"] = adId;
    json["adSize"] = static_cast<int>(adSize);

    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createBannerAd, json.dump());
    if (response == "false") {
        return nullptr;
    }
    return std::shared_ptr<AdViewInterface>(new BannerAd(this, adId));
}

bool Self::destroyBannerAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyBannerAd, adId);
    return response == "true";
}

std::shared_ptr<AdViewInterface>
Self::createNativeAd(const NativeAdBuilder& builder) {
    nlohmann::json json;
    json["adId"] = builder.adId_;
    json["layoutName"] = builder.layoutName_;
    json["icon"] = builder.icon_;
    json["title"] = builder.title_;
    json["media"] = builder.media_;
    json["socialContext"] = builder.socialContext_;
    json["adChoices"] = builder.adChoices_;
    json["body"] = builder.body_;
    json["action"] = builder.action_;

    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createNativeAd, json.dump());
    if (response == "false") {
        return nullptr;
    }
    return std::shared_ptr<AdViewInterface>(new NativeAd(this, builder.adId_));
}

bool Self::destroyNativeAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyNativeAd, adId);
    return response == "true";
}

void Self::cacheInterstitialAd(const std::string& adId) const {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__cacheInterstitialAd, adId);
}

bool Self::showInterstitialAd() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto&& response = bridge.call(k__showInterstitialAd);
    return response == "true";
}
} // namespace facebook
} // namespace ee
