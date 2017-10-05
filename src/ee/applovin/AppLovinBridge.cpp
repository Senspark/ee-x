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
constexpr auto k__initialize            = "AppLovin_initialize";
constexpr auto k__isInterstitialAdReady = "AppLovin_isInterstitialAdReady";
constexpr auto k__showInterstitialAd    = "AppLovin_showInterstitialAd";
constexpr auto k__showRewardedVideo     = "AppLovin_showRewardedVideo";
constexpr auto k__cppCallback           = "AppLovin_cppCallback";
// clang-format on
} // namespace

const std::string Self::DefaultPlacementId = "AppLovin_placementId";

Self::AppLovin() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            auto result = json["result"].get<bool>();
            invokeRewardedVideoCallback(result, DefaultPlacementId);
            return "";
        },
        k__initialize);
}

Self::~AppLovin() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__cppCallback);
}

void Self::initialize() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize);
}

bool Self::isInterstitialAdReady() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__isInterstitialAdReady);
    return result == "true";
}

bool Self::showInterstitialAd() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__showInterstitialAd);
    return result == "true";
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (placementId != DefaultPlacementId) {
        return false;
    }
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__showRewardedVideo);
    return result == "true";
}
} // namespace applovin
} // namespace ee
