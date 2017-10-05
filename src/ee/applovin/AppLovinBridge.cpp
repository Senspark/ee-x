//
//  ALovinAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/applovin/AppLovinBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <nlohmann/json.hpp>

namespace ee {
namespace applovin {
using Self = AppLovin;

namespace {
// clang-format off
constexpr auto k__ALovinads_initALovinAds       = "k__ALovinads_initALovinAds";
constexpr auto k__ALovinads_isInterstitialReady = "k__ALovinads_isInterstitialReady";
constexpr auto k__ALovinads_showInterstitial    = "k__ALovinads_showInterstitial";
constexpr auto k__ALovinads_isRewardVideoReady  = "k__ALovinads_isRewardVideoReady";
constexpr auto k__ALovinads_showRewardVideo     = "k__ALovinads_showRewardVideo";
constexpr auto k__AppLovin_cpp_callback         = "__ALovinAds_callback";
// clang-format on
} // namespace

Self::AppLovin() {
    callback_ = nullptr;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        std::bind(&Self::doCallBack, this, std::placeholders::_1),
        k__AppLovin_cpp_callback);
}

Self::~AppLovin() {
    core::MessageBridge::getInstance().deregisterHandler(
        k__AppLovin_cpp_callback);
}

std::string Self::doCallBack(const std::string& msg) const {
    if (callback_) {
        auto json = nlohmann::json::parse(msg);

        auto code = static_cast<ALovinAdsResultCode>(json["code"].get<int>());
        std::string message = json["placement"];

        callback_(code, message);
    }

    return "";
}

void Self::initialize() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__ALovinads_initALovinAds);
}

bool Self::isInterstitialReady() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__ALovinads_isInterstitialReady);
    return result == "true";
}

bool Self::showInterstitial() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__ALovinads_showInterstitial);
    return result == "true";
}

bool Self::isRewardVideoReady() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__ALovinads_isRewardVideoReady);
    return result == "true";
}

bool Self::showRewardVideo() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__ALovinads_showRewardVideo);
    return result == "true";
}
} // namespace applovin
} // namespace ee
