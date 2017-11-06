#include "ee/vungle/VungleBridge.hpp"
#include "ee/ads/NullRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/vungle/internal/VungleRewardedVideo.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace vungle {
using Self = Vungle;

namespace {
// clang-format off
constexpr auto k__initialize        = "Vungle_initialize";
constexpr auto k__hasRewardedVideo  = "Vungle_hasRewardedVideo";
constexpr auto k__showRewardedVideo = "Vungle_showRewardedVideo";
constexpr auto k__onStart           = "Vungle_onStart";
constexpr auto k__onEnd             = "Vungle_onEnd";
constexpr auto k__onUnavailable     = "Vungle_onUnavailable";
// clang-format on
} // namespace

Self::Vungle() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    errored_ = false;
    rewardedVideo_ = nullptr;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onStart();
            return "";
        },
        k__onStart);
    bridge.registerHandler(
        [this](const std::string& message) {
            onEnd(core::toBool(message));
            return "";
        },
        k__onEnd);
    bridge.registerHandler(
        [this](const std::string& message) {
            onUnavailable();
            return "";
        },
        k__onUnavailable);
}

Self::~Vungle() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onStart);
    bridge.deregisterHandler(k__onEnd);
    bridge.deregisterHandler(k__onUnavailable);
}

void Self::initialize(const std::string& gameId) {
    Logger::getSystemLogger().debug("%s: gameId = %s", __PRETTY_FUNCTION__,
                                    gameId.c_str());
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, gameId);
}

std::shared_ptr<RewardedVideoInterface> Self::createRewardedVideo() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    if (rewardedVideo_ != nullptr) {
        return std::make_shared<NullRewardedVideo>();
    }
    auto result = new RewardedVideo(this);
    rewardedVideo_ = result;
    return std::shared_ptr<RewardedVideoInterface>(result);
}

bool Self::destroyRewardedVideo() {
    Logger::getSystemLogger().debug("%s: %p", __PRETTY_FUNCTION__,
                                    rewardedVideo_);
    if (rewardedVideo_ == nullptr) {
        return false;
    }
    rewardedVideo_ = nullptr;
    return true;
}

bool Self::hasRewardedVideo() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto result = bridge.call(k__hasRewardedVideo);
    return core::toBool(result);
}

bool Self::showRewardedVideo() {
    if (not hasRewardedVideo()) {
        return false;
    }
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    errored_ = false;
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__showRewardedVideo);
    return not errored_ && core::toBool(response);
}

void Self::onStart() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
}

void Self::onEnd(bool wasSuccessfulView) {
    Logger::getSystemLogger().debug("%s: %s", __PRETTY_FUNCTION__,
                                    core::toString(wasSuccessfulView).c_str());
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.setRewardedVideoResult(wasSuccessfulView);
    assert(successful);

    /*
     Don't care which ad is displaying.
    if (rewardedVideo_ == nullptr) {
        // Other mediation network.

    } else {
        assert(not errored_);
        rewardedVideo_->setResult(wasSuccessfulView);
        auto successful = mediation.finishRewardedVideo(rewardedVideo_);
        assert(successful);
    }
     */
}

void Self::onUnavailable() {
    Logger::getSystemLogger().debug(__PRETTY_FUNCTION__);
    if (not errored_) {
        errored_ = true;
    }
}
} // namespace vungle
} // namespace ee
