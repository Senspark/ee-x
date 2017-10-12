//
//  UnityAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/unityads/UnityAdsBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/unityads/internal/UnityInterstitialAd.hpp"
#include "ee/unityads/internal/UnityRewardedVideo.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace unityads {
using Self = UnityAds;

namespace {
// clang-format off
constexpr auto k__initialize           = "UnityAds_initialize";
constexpr auto k__setDebugModeEnabled  = "UnityAds_setDebugModeEnabled";
constexpr auto k__isRewardedVideoReady = "UnityAds_isRewardedVideoReady";
constexpr auto k__showRewardedVideo    = "UnityAds_showRewardedVideo";
constexpr auto k__onError              = "UnityAds_onError";
constexpr auto k__onSkipped            = "UnityAds_onSkipped";
constexpr auto k__onFinished           = "UnityAds_onFinished";
// clang-format on
} // namespace

Self::UnityAds() {
    errored_ = false;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onError(message);
            return "";
        },
        k__onError);
    bridge.registerHandler(
        [this](const std::string& message) {
            onSkipped(message);
            return "";
        },
        k__onSkipped);
    bridge.registerHandler(
        [this](const std::string& message) {
            onFinished(message);
            return "";
        },
        k__onFinished);
}

Self::~UnityAds() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onError);
    bridge.deregisterHandler(k__onSkipped);
    bridge.deregisterHandler(k__onFinished);
}

void Self::initialize(const std::string& gameId, bool testModeEnabled) {
    nlohmann::json json;
    json["gameId"] = gameId;
    json["testModeEnabled"] = testModeEnabled ? "true" : "false";

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, json.dump());
}

void Self::setDebugModeEnabled(bool enabled) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__setDebugModeEnabled, enabled ? "true" : "false");
}

std::shared_ptr<RewardedVideoInterface>
Self::createRewardedVideo(const std::string& placementId) {
    if (rewardedVideos_.count(placementId) != 0) {
        return nullptr;
    }
    auto result = new RewardedVideo(this, placementId);
    rewardedVideos_[placementId] = result;
    return std::shared_ptr<RewardedVideoInterface>(result);
}

bool Self::destroyRewardedVideo(const std::string& placementId) {
    if (rewardedVideos_.count(placementId) == 0) {
        return false;
    }
    rewardedVideos_.erase(placementId);
    return true;
}

std::shared_ptr<InterstitialAdInterface>
Self::createInterstitialAd(const std::string& placementId) {
    if (interstitialAds_.count(placementId) != 0) {
        return nullptr;
    }
    auto result = new InterstitialAd(this, placementId);
    interstitialAds_[placementId] = result;
    return std::shared_ptr<InterstitialAdInterface>(result);
}

bool Self::destroyInterstitialAd(const std::string& placementId) {
    if (interstitialAds_.count(placementId) == 0) {
        return false;
    }
    interstitialAds_.erase(placementId);
    return true;
}

bool Self::isRewardedVideoReady(const std::string& placementId) const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__isRewardedVideoReady, placementId);
    return response == "true";
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (not isRewardedVideoReady(placementId)) {
        return false;
    }
    errored_ = false;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__showRewardedVideo, placementId);
    return not errored_;
}

void Self::onError(const std::string& placementId) {
    if (not errored_) {
        errored_ = true;
        return;
    }
    onSkipped(placementId);
}

void Self::onSkipped(const std::string& placementId) {
    if (rewardedVideos_.count(placementId)) {
        auto ad = rewardedVideos_.at(placementId);
        ad->setResult(false);
        return;
    }
    if (interstitialAds_.count(placementId)) {
        auto ad = interstitialAds_.at(placementId);
        ad->setDone();
        return;
    }
    assert(false);
}

void Self::onFinished(const std::string& placementId) {
    if (rewardedVideos_.count(placementId)) {
        auto ad = rewardedVideos_.at(placementId);
        ad->setResult(true);
        return;
    }
    if (interstitialAds_.count(placementId)) {
        auto ad = interstitialAds_.at(placementId);
        ad->setDone();
        return;
    }
    assert(false);
}
} // namespace unityads
} // namespace ee
