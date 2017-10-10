#include <cassert>

#include "ee/core/internal/MessageBridge.hpp"
#include "ee/ironsource/IronSourceBridge.hpp"
#include "ee/ironsource/IronSourceRewardedVideo.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace ironsource {
using Self = IronSource;

namespace {
// clang-format off
constexpr auto k__initialize        = "IronSource_initialize";
constexpr auto k__hasRewardedVideo  = "IronSource_hasRewardedVideo";
constexpr auto k__showRewardedVideo = "IronSource_showRewardedVideo";
constexpr auto k__onRewarded        = "IronSource_onRewarded";
constexpr auto k__onFailed          = "IronSource_onFailed";
constexpr auto k__onOpened          = "IronSource_onOpened";
constexpr auto k__onClosed          = "IronSource_onClosed";
// clang-format on
} // namespace

Self::IronSource() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onRewarded(message);
            return "";
        },
        k__onRewarded);
    bridge.registerHandler(
        [this](const std::string& message) {
            onFailed();
            return "";
        },
        k__onFailed);
    bridge.registerHandler(
        [this](const std::string& message) {
            onOpened();
            return "";
        },
        k__onOpened);
    bridge.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed);
}

Self::~IronSource() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onRewarded);
    bridge.deregisterHandler(k__onFailed);
    bridge.deregisterHandler(k__onOpened);
    bridge.deregisterHandler(k__onClosed);
}

void Self::initialize(const std::string& gameId) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, gameId);
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

bool Self::hasRewardedVideo() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__hasRewardedVideo);
    return result == "true";
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (not hasRewardedVideo()) {
        return false;
    }
    succesfullyDisplay_ = true;
    rewarded_ = false;
    placementId_ = placementId;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__showRewardedVideo, placementId);
    return succesfullyDisplay_;
}

void Self::onRewarded(const std::string& placementId) {
    rewarded_ = true;
    assert(placementId_ == placementId);
}

void Self::onFailed() {
    if (succesfullyDisplay_) {
        succesfullyDisplay_ = false;
    }
}

void Self::onOpened() {
    //
}

void Self::onClosed() {
    auto ad = rewardedVideos_.at(placementId_);
    if (rewarded_) {
        ad->setResult(true);
    } else {
        ad->setResult(false);
    }
}
} // namespace ironsource
} // namespace ee
