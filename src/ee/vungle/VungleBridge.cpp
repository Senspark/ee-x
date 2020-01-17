#include "ee/vungle/VungleBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/ads/NullRewardedVideo.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/core/internal/SharedPtrUtils.hpp>

#include "ee/vungle/private/VungleRewardedVideo.hpp"

namespace ee {
namespace vungle {
using Self = Vungle;

namespace {
// clang-format off
constexpr auto k__initialize        = "Vungle_initialize";
constexpr auto k__hasRewardedVideo  = "Vungle_hasRewardedVideo";
constexpr auto k__showRewardedVideo = "Vungle_showRewardedVideo";
constexpr auto k__loadVideoAd       = "Vungle_loadVideoAd";
constexpr auto k__onStart           = "Vungle_onStart";
constexpr auto k__onEnd             = "Vungle_onEnd";
constexpr auto k__onUnavailable     = "Vungle_onUnavailable";
// clang-format on
} // namespace

Self::Vungle()
    : Self(Logger::getSystemLogger()) {}

Self::Vungle(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug(__PRETTY_FUNCTION__);
    errored_ = false;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onStart();
            return "";
        },
        k__onStart);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onEnd(core::toBool(message));
            return "";
        },
        k__onEnd);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onUnavailable();
            return "";
        },
        k__onUnavailable);
}

Self::~Vungle() {
    logger_.debug(__PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onStart);
    bridge_.deregisterHandler(k__onEnd);
    bridge_.deregisterHandler(k__onUnavailable);
}

void Self::initialize(const std::string& gameId) {
    logger_.debug("%s: gameId = %s", __PRETTY_FUNCTION__, gameId.c_str());

    nlohmann::json json;
    json["gameId"] = gameId;
    bridge_.call(k__initialize, json.dump());
}

void Self::initialize(const std::string& gameId,
                      const std::string& placementId) {
    initialize(gameId);
}

std::shared_ptr<IRewardedVideo>
Self::createRewardedVideo(const std::string& placementId) {
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    if (rewardedVideos_.count(placementId) != 0) {
        return core::makeShared<NullRewardedVideo>(logger_);
    }
    auto result = new RewardedVideo(logger_, this, placementId);
    rewardedVideos_[placementId] = result;
    return std::shared_ptr<IRewardedVideo>(result);
}

bool Self::destroyRewardedVideo(const std::string& placementId) {
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    if (rewardedVideos_.count(placementId) == 0) {
        return false;
    }
    rewardedVideos_.erase(placementId);
    return true;
}

void Self::loadVideoAd(const std::string& placementId) const {
    logger_.debug("%s: load placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    bridge_.call(k__loadVideoAd, placementId);
}

bool Self::hasRewardedVideo(const std::string& placementId) const {
    auto result = bridge_.call(k__hasRewardedVideo, placementId);
    return core::toBool(result);
}

bool Self::showRewardedVideo(const std::string& placementId) {
    if (not hasRewardedVideo(placementId)) {
        return false;
    }
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    errored_ = false;
    auto response = bridge_.call(k__showRewardedVideo, placementId);
    return not errored_ && core::toBool(response);
}

void Self::onStart() {
    logger_.debug(__PRETTY_FUNCTION__);
}

void Self::onEnd(bool wasSuccessfulView) {
    logger_.debug("%s: %s", __PRETTY_FUNCTION__,
                  core::toString(wasSuccessfulView).c_str());
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.finishRewardedVideo(wasSuccessfulView);
    assert(successful);
}

void Self::onUnavailable() {
    logger_.debug(__PRETTY_FUNCTION__);
    if (not errored_) {
        errored_ = true;
    }
}
} // namespace vungle
} // namespace ee
