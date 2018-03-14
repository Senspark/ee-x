#include <cassert>

#include "ee/ads/NullRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/ironsource/IronSourceBridge.hpp"
#include "ee/ironsource/internal/IronSourceRewardedVideo.hpp"

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

Self::IronSource()
    : bridge_(MessageBridge::getInstance()) {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    errored_ = false;
    rewarded_ = false;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewarded(message);
            return "";
        },
        k__onRewarded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailed();
            return "";
        },
        k__onFailed);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onOpened();
            return "";
        },
        k__onOpened);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed);
}

Self::~IronSource() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onRewarded);
    bridge_.deregisterHandler(k__onFailed);
    bridge_.deregisterHandler(k__onOpened);
    bridge_.deregisterHandler(k__onClosed);
}

void Self::initialize(const std::string& gameId) {
    Logger::getSystemLogger().debug("%s: gameId = %s", __PRETTY_FUNCTION__,
                                    gameId.c_str());
    bridge_.call(k__initialize, gameId);
}

std::shared_ptr<IRewardedVideo>
Self::createRewardedVideo(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (rewardedVideos_.count(placementId) != 0) {
        return std::make_shared<NullRewardedVideo>();
    }
    auto result = new RewardedVideo(this, placementId);
    rewardedVideos_[placementId] = result;
    return std::shared_ptr<IRewardedVideo>(result);
}

bool Self::destroyRewardedVideo(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    if (rewardedVideos_.count(placementId) == 0) {
        return false;
    }
    rewardedVideos_.erase(placementId);
    return true;
}

bool Self::hasRewardedVideo() const {
    auto response = bridge_.call(k__hasRewardedVideo);
    return core::toBool(response);
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (not hasRewardedVideo()) {
        return false;
    }
    errored_ = false;
    rewarded_ = false;
    bridge_.call(k__showRewardedVideo, placementId);
    return not errored_;
}

void Self::onRewarded(const std::string& placementId) {
    Logger::getSystemLogger().debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                                    placementId.c_str());
    rewarded_ = true;
}

void Self::onFailed() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    if (not errored_) {
        errored_ = true;
    }
}

void Self::onOpened() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    rewarded_ = false;
}

void Self::onClosed() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();

    /*
     Don't care which ad is displaying.
    if (rewardedVideos_.count(placementId_)) {
        auto ad = rewardedVideos_.at(placementId_);
        if (rewarded_) {
            ad->setResult(true);
        } else {
            ad->setResult(false);
        }
        auto successful = mediation.finishRewardedVideo(ad);
        assert(successful);
        return;
    }
     */

    // Other mediation network.
    auto successful = mediation.setRewardedVideoResult(rewarded_);
    assert(successful);
}
} // namespace ironsource
} // namespace ee
