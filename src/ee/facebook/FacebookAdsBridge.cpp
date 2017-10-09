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

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
FacebookAds::FacebookAds() {
    callback_ = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&FacebookAds::onAdCallback, this, std::placeholders::_1),
        "__facebookads_callback");
}

FacebookAds::~FacebookAds() {
    core::MessageBridge::getInstance().deregisterHandler(
        "__facebookads_callback");
}

void FacebookAds::initFBAdsInterstitial(
    const std::string& interstitialAdId) const {
    nlohmann::json json;
    json["InterstitialID"] = interstitialAdId;
    core::MessageBridge::getInstance().call(
        "__facebookads_initFBAdsInterstitial", json.dump());
}

void FacebookAds::initFBAdsNativeAds(const std::string& nativeAdId,
                                     const std::string& layout) const {
    nlohmann::json json;
    json["NativeID"] = nativeAdId;
    json["layout"] = layout;
    core::MessageBridge::getInstance().call("__facebookads_initFBAdsNativeAds",
                                            json.dump());
}

void FacebookAds::initFBAdsBanner(const std::string& bannerAdId) const {
    nlohmann::json json;
    json["BannerID"] = bannerAdId;
    core::MessageBridge::getInstance().call("__facebookads_initFBAdsBanner",
                                            json.dump());
}

void FacebookAds::cacheRewardedAd(const std::string& adId) const {
    nlohmann::json json;
    json["adsID"] = adId;
    core::MessageBridge::getInstance().call("__facebookads_cacheRewardedAd",
                                            json.dump());
}

void FacebookAds::cacheInterstitialAd(const std::string& adId) const {
    nlohmann::json json;
    json["adsID"] = adId;
    core::MessageBridge::getInstance().call("__facebookads_cacheInterstitialAd",
                                            json.dump());
}

void FacebookAds::showBannerAd(const std::string& adId, int position) const {
    nlohmann::json json;
    json["adsID"] = adId;
    json["pos"] = position;
    core::MessageBridge::getInstance().call("__facebookads_showBannerAd",
                                            json.dump());
}

void FacebookAds::hideBannerAd() const {
    core::MessageBridge::getInstance().call("__facebookads_hideBannerAd");
}

bool FacebookAds::hasInterstitialAd() const {
    auto result = core::MessageBridge::getInstance().call(
        "__facebookads_hasInterstitialAd");
    return result == "true";
}

bool FacebookAds::hasRewardedAd() const {
    auto result =
        core::MessageBridge::getInstance().call("__facebookads_hasRewardedAd");
    return result == "true";
}

bool FacebookAds::hasNativeAd(const std::string& adId) const {
    nlohmann::json json;
    json["adsID"] = adId;
    auto result = core::MessageBridge::getInstance().call(
        "__facebookads_hasNativeAd", json.dump());
    return result == "true";
}

void FacebookAds::showInterstitialAd() const {
    core::MessageBridge::getInstance().call("__facebookads_showInterstitialAd");
}

void FacebookAds::showRewardedAd() const {
    core::MessageBridge::getInstance().call("__facebookads_showRewardedAd");
}

void FacebookAds::hideNativeAd(const std::string& adId) {
    nlohmann::json json;
    json["adsID"] = adId;
    core::MessageBridge::getInstance().call("__facebookads_hideNativeAd",
                                            json.dump());
}

void FacebookAds::showNativeAd(const std::string& adId, int width, int height,
                               int x, int y) {
    nlohmann::json json;
    json["adsID"] = adId;
    json["width"] = width;
    json["height"] = height;
    json["x"] = x;
    json["y"] = y;
    core::MessageBridge::getInstance().call("__facebookads_showNativeAd",
                                            json.dump());
}

std::string FacebookAds::onAdCallback(const std::string& msg) const {
    if (callback_) {
        callback_(msg);
    }
    return "";
}
} // namespace crashlytics
} // namespace ee
