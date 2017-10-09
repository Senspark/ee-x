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
constexpr auto k__createBannerAd = "FacebookAds_createBannerAd";
constexpr auto k__destroyBannerAd = "FacebookAds_destroyBannerAd";

constexpr auto k__createNativeAd = "FacebookAds_createNativeAd";
constexpr auto k__destroyNativeAd = "FacebookAds_destroyNativeAd";

constexpr auto k__cacheInterstitialAd = "FacebookAds_cacheInterstitialAd";
constexpr auto k__showInterstitialAd = "FacebookAds_showInterstitialAd";

constexpr auto k__cppCallback = "FacebookAds_cppCallback";
} // namespace

FacebookAds::FacebookAds() {}

FacebookAds::~FacebookAds() {}

std::shared_ptr<FacebookBannerAd>
Self::createBannerAd(const std::string& adId, FacebookBannerAdSize adSize) {
    nlohmann::json json;
    json["adId"] = adId;
    json["adSize"] = static_cast<int>(adSize);

    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createBannerAd, json.dump());
    if (response == "false") {
        return nullptr;
    }
    return std::shared_ptr<FacebookBannerAd>(new FacebookBannerAd(this, adId));
}

bool Self::destroyBannerAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyBannerAd, adId);
    return response == "true";
}

std::shared_ptr<FacebookNativeAd>
Self::createNativeAd(const FacebookNativeAdBuilder& builder) {
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
    return std::shared_ptr<FacebookNativeAd>(
        new FacebookNativeAd(this, builder.adId_));
}

bool Self::destroyNativead(const std::string& adId) {
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
