#include <cassert>

#include "ee/ads/NullInterstitialAd.hpp"
#include "ee/ads/NullRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/SpinLock.hpp"
#include "ee/ironsource/IronSourceBridge.hpp"
#include "ee/ironsource/internal/IronSourceInterstitialAd.hpp"
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
    
constexpr auto k__loadInterstitial   = "IronSource_loadInterstitial";
constexpr auto k__hasInterstitial   = "IronSource_hasInterstitial";
constexpr auto k__showInterstitial  = "IronSource_showInterstitial";
constexpr auto k__onRewarded        = "IronSource_onRewarded";
constexpr auto k__onFailed          = "IronSource_onFailed";
constexpr auto k__onOpened          = "IronSource_onOpened";
constexpr auto k__onClosed          = "IronSource_onClosed";
    
constexpr auto k__onInterstitialFailed         = "IronSource_onInterstitialFailed";
constexpr auto k__onInterstitialOpened          = "IronSource_onInterstitialOpened";
constexpr auto k__onInterstitialClosed          = "IronSource_onInterstitialClosed";
constexpr auto k__onInterstitialClicked         = "IronSource_onInterstitialClicked";
// clang-format on
} // namespace

Self::IronSource()
    : Self(Logger::getSystemLogger()) {}

Self::IronSource(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , _closeTimeout(0.0f)
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
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

    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialOpened();
            return "";
        },
        k__onInterstitialOpened);

    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialFailed();
            return "";
        },
        k__onInterstitialFailed);

    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialClosed();
            return "";
        },
        k__onInterstitialClosed);

    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialClicked();
            return "";
        },
        k__onInterstitialClicked);

    handlerLock_ = std::make_unique<core::SpinLock>();
}

Self::~IronSource() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onRewarded);
    bridge_.deregisterHandler(k__onFailed);
    bridge_.deregisterHandler(k__onOpened);
    bridge_.deregisterHandler(k__onClosed);

    bridge_.deregisterHandler(k__onInterstitialOpened);
    bridge_.deregisterHandler(k__onInterstitialFailed);
    bridge_.deregisterHandler(k__onInterstitialClosed);
    bridge_.deregisterHandler(k__onInterstitialClicked);
}

void Self::initialize(const std::string& gameId) {
    logger_.debug("%s: gameId = %s", __PRETTY_FUNCTION__, gameId.c_str());
    bridge_.call(k__initialize, gameId);
}

std::shared_ptr<IRewardedVideo>
Self::createRewardedVideo(const std::string& placementId) {
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    if (rewardedVideos_.count(placementId) != 0) {
        return std::make_shared<NullRewardedVideo>(logger_);
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

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& placementId) {
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    if (interstitialAds_.count(placementId) != 0) {
        return std::make_shared<NullInterstitialAd>();
    }
    auto result = new InterstitialAd(logger_, this, placementId);
    interstitialAds_[placementId] = result;
    return std::shared_ptr<IInterstitialAd>(result);
}

bool Self::destroyInterstitialAd(const std::string& placementId) {
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    if (interstitialAds_.count(placementId) == 0) {
        return false;
    }
    interstitialAds_.erase(placementId);
    return true;
}

void Self::loadInterstitial() {
    bridge_.call(k__loadInterstitial);
}

bool Self::hasInterstitial() const {
    auto response = bridge_.call(k__hasInterstitial);
    return core::toBool(response);
}

bool Self::showInterstitial(const std::string& placementId) {
    if (not hasInterstitial()) {
        return false;
    }
    placementId_ = placementId;
    rewarded_ = false;
    bridge_.call(k__showInterstitial, placementId);
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
    rewarded_ = false;
    bridge_.call(k__showRewardedVideo, placementId);
    return true;
}

void Self::onRewarded(const std::string& placementId) {
    std::lock_guard<core::SpinLock> guard(*handlerLock_);
    logger_.debug("%s: placementId = %s", __PRETTY_FUNCTION__,
                  placementId.c_str());
    rewarded_ = true;
}

void Self::onFailed() {
    std::lock_guard<core::SpinLock> guard(*handlerLock_);
    logger_.debug("%s", __PRETTY_FUNCTION__);

    rewarded_ = false;
    doRewardAndFinishAds();
}

void Self::onOpened() {
    std::lock_guard<core::SpinLock> guard(*handlerLock_);
    logger_.debug("%s", __PRETTY_FUNCTION__);
    rewarded_ = false;
}

void Self::onClosed() {
    std::lock_guard<core::SpinLock> guard(*handlerLock_);
    logger_.debug("%s", __PRETTY_FUNCTION__);

    // wait for reward
    // if out of time just callback failed
    ee::core::runOnUiThreadDelayed(
        [this] {
            std::lock_guard<core::SpinLock> guard(*handlerLock_);
            doRewardAndFinishAds();
        },
        _closeTimeout);
}

void Self::doRewardAndFinishAds() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();

    // Other mediation network.
    auto wasInterstitialAd = mediation.setInterstitialAdDone();
    auto wasRewardedVideo = mediation.finishRewardedVideo(rewarded_);

    assert(wasInterstitialAd || wasRewardedVideo);
}

#pragma mark - For Interstitial

void Self::onInterstitialOpened() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

void Self::onInterstitialFailed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    onInterstitialClosed();
}

void Self::onInterstitialClosed() {
    std::lock_guard<core::SpinLock> guard(*handlerLock_);
    logger_.debug("%s", __PRETTY_FUNCTION__);
    //    auto&& mediation = ads::MediationManager::getInstance();
    //
    //    auto successful = mediation.finishInterstitialAd();
    //    assert(successful);
    doRewardAndFinishAds();
}

void Self::onInterstitialClicked() {
    auto interstitialAd = interstitialAds_[placementId_];
    if (interstitialAd) {
        interstitialAd->doOnClicked();
    }
}

#pragma mark - Config

void Self::setCloseTimeout(float timeout) {
    std::lock_guard<core::SpinLock> guard(*handlerLock_);
    _closeTimeout = timeout < 1 ? 1 : timeout;
}
} // namespace ironsource
} // namespace ee
