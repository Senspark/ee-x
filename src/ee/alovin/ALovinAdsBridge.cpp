//
//  ALovinAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/alovin/ALovinAdsBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace alovinads {
ALovinAds::ALovinAds() {
    callback_ = nullptr;
    core::MessageBridge::getInstance().registerHandler(
        std::bind(&ALovinAds::doCallBack, this, std::placeholders::_1),
        "__ALovinAds_callback");
}

ALovinAds::~ALovinAds() {
    core::MessageBridge::getInstance().deregisterHandler(
        "__ALovinAds_callback");
}

std::string ALovinAds::doCallBack(const std::string& msg) const {
    if (callback_) {
        auto json = nlohmann::json::parse(msg);

        auto code = static_cast<ALovinAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];

        callback_(code, message);
    }

    return "";
}

void ALovinAds::initALovinAds() {
    nlohmann::json json;
    core::MessageBridge::getInstance().call("k__ALovinads_initALovinAds",
                                            json.dump());
}

bool ALovinAds::isInterstitialReady() {
    nlohmann::json json;
    auto result = core::MessageBridge::getInstance().call(
        "k__ALovinads_isInterstitialReady", json.dump());
    return result == "true";
}

void ALovinAds::showInterstitial() {
    nlohmann::json json;
    core::MessageBridge::getInstance().call("k__ALovinads_showInterstitial",
                                            json.dump());
}

bool ALovinAds::isRewardVideoReady() {
    nlohmann::json json;
    auto result = core::MessageBridge::getInstance().call(
        "k__ALovinads_isRewardVideoReady", json.dump());
    return result == "true";
}

void ALovinAds::showRewardVideo() {
    nlohmann::json json;
    core::MessageBridge::getInstance().call("k__ALovinads_showRewardVideo",
                                            json.dump());
}

} // namespace ALovinads
} // namespace ee
