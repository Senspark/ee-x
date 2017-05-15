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

#include <nlohmann/json.hpp>

namespace ee {
namespace facebook {

void FacebookAds::initFBAds(const std::string& InterstitialID,
                            const std::string& NativeID,
                            const std::string& BannerID) const {
    nlohmann::json json;
    json["InterstitialID"] = InterstitialID;
    json["NativeID"] = NativeID;
    json["BannerID"] = BannerID;
    core::MessageBridge::getInstance().call("__facebookads_initFBAds",
                                            json.dump());
}

void FacebookAds::cacheRewardedAd(const std::string& adsID) const {
    nlohmann::json json;
    json["adsID"] = adsID;
    core::MessageBridge::getInstance().call("__facebookads_cacheRewardedAd",
                                            json.dump());
}

void FacebookAds::cacheInterstitialAd(const std::string& adsID) const {
    nlohmann::json json;
    json["adsID"] = adsID;
    core::MessageBridge::getInstance().call("__facebookads_cacheInterstitialAd",
                                            json.dump());
}

void FacebookAds::showBannerAd(const std::string& adsID, int pos) const {
    nlohmann::json json;
    json["adsID"] = adsID;
    json["pos"] = pos;
    core::MessageBridge::getInstance().call("__facebookads_showBannerAd",
                                            json.dump());
}

void FacebookAds::hideBannerAd() const {
    core::MessageBridge::getInstance().call("__facebookads_hideBannerAd");
}

bool FacebookAds::hasInterstitialAd() const {
    auto result = core::MessageBridge::getInstance().call(
        "__facebookads_hasInterstitialAd");
    auto json = nlohmann::json::parse(result);
    return json["result"].get<bool>();
}

bool FacebookAds::hasRewardedAd() const {
    auto result =
        core::MessageBridge::getInstance().call("__facebookads_hasRewardedAd");
    auto json = nlohmann::json::parse(result);
    return json["result"].get<bool>();
}

void FacebookAds::showInterstitialAd() const {
    core::MessageBridge::getInstance().call("__facebookads_showInterstitialAd");
}

void FacebookAds::showRewardedAd() const {
    core::MessageBridge::getInstance().call("__facebookads_showRewardedAd");
}

void FacebookAds::showNativeExpressAd(const std::string& adsID, int width,
                                      int height, int pos) {
    nlohmann::json json;
    json["adsID"] = adsID;
    json["width"] = width;
    json["height"] = height;
    json["pos"] = pos;
    core::MessageBridge::getInstance().call("__facebookads_showNativeExpressAd",
                                            json.dump());
}

void FacebookAds::showNativeExpressAdWithDeltaPosition(const std::string& adsId,
                                                       int width, int height,
                                                       int deltaX, int deltaY) {
    nlohmann::json json;
    json["adsId"] = adsId;
    json["width"] = width;
    json["height"] = height;
    json["deltaX"] = deltaX;
    json["deltaY"] = deltaY;
    core::MessageBridge::getInstance().call(
        "__facebookads_showNativeExpressAdWithDeltaPosition", json.dump());
}

void FacebookAds::hideNativeExpressAd() {
    core::MessageBridge::getInstance().call(
        "__facebookads_hideNativeExpressAd");
}

void FacebookAds::showNativeAdvancedAd(const std::string& adsID, int width,
                                       int height, int x, int y) {
    nlohmann::json json;
    json["adsID"] = adsID;
    json["width"] = width;
    json["height"] = height;
    json["x"] = x;
    json["y"] = y;
    core::MessageBridge::getInstance().call(
        "__facebookads_showNativeAdvancedAd", json.dump());
}

void FacebookAds::hideNativeAdvancedAd() {
    core::MessageBridge::getInstance().call(
        "__facebookads_hideNativeAdvancedAd");
}

void FacebookAds::onAdsCallback(int code, const std::string& msg) const {
    nlohmann::json json;
    json["code"] = code;
    json["msg"] = msg;
    core::MessageBridge::getInstance().call("__facebookads_onAdsCallback",
                                            json.dump());
}

} // namespace crashlytics
} // namespace ee
