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
FacebookAds::FacebookAds() {
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&FacebookAds::onAdsCallback, this, std::placeholders::_1),
        "__facebookads_callback");
}

FacebookAds::~FacebookAds() {
    core::MessageBridge::getInstance().deregisterHandlere(
        "__facebookads_callback");
}

void FacebookAds::initFBAdsInterstitial(const std::string& InterstitialID) const {
    nlohmann::json json;
    json["InterstitialID"] = InterstitialID;
    core::MessageBridge::getInstance().call(
        "__facebookads_initFBAdsInterstitial", json.dump());
}

void FacebookAds::initFBAdsNativeAds(const std::string& NativeID, const std::string& layout) const {
    nlohmann::json json;
    json["NativeID"] = NativeID;
    json["layout"] = layout;
    core::MessageBridge::getInstance().call("__facebookads_initFBAdsNativeAds",
                                            json.dump());
}

void FacebookAds::initFBAdsBanner(const std::string& BannerID) const {
    nlohmann::json json;
    json["BannerID"] = BannerID;
    core::MessageBridge::getInstance().call("__facebookads_initFBAdsBanner",
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
    //    auto result = core::MessageBridge::getInstance().call(
    //        "__facebookads_hasInterstitialAd");
    //    auto json = nlohmann::json::parse(result);
    //    return json["result"].get<bool>();
    return true;
}

bool FacebookAds::hasRewardedAd() const {
    //    auto result =
    //        core::MessageBridge::getInstance().call("__facebookads_hasRewardedAd");
    //    auto json = nlohmann::json::parse(result);
    //    return json["result"].get<bool>();
    return true;
}

void FacebookAds::showInterstitialAd() const {
    core::MessageBridge::getInstance().call("__facebookads_showInterstitialAd");
}

void FacebookAds::showRewardedAd() const {
    core::MessageBridge::getInstance().call("__facebookads_showRewardedAd");
}

void FacebookAds::hideNativeAd(const std::string& adsID) {
    nlohmann::json json;
    json["adsID"] = adsID;
    core::MessageBridge::getInstance().call(
        "__facebookads_hideNativeAd", json.dump());
}

void FacebookAds::showNativeAd(const std::string& adsID, int width,
                                       int height, int x, int y) {
    nlohmann::json json;
    json["adsID"] = adsID;
    json["width"] = width;
    json["height"] = height;
    json["x"] = x;
    json["y"] = y;
    core::MessageBridge::getInstance().call(
        "__facebookads_showNativeAd", json.dump());
}

std::string FacebookAds::onAdsCallback(const std::string& msg) const {

    if (_callback) {
        _callback(msg);
    }

    return "";
}

} // namespace crashlytics
} // namespace ee
