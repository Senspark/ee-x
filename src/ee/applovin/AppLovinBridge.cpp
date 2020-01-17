//
//  ALovinAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/applovin/AppLovinBridge.hpp"

#include <cassert>

#include <ee/nlohmann/json.hpp>

#include <ee/ads/NullRewardedVideo.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/applovin/private/AppLovinRewardedVideo.hpp"

namespace ee {
namespace applovin {
using Self = AppLovin;

enum class Error {
    // Loading & Displaying Ads

    // Indicates that no ads are currently eligible for your device & location.
    NoFill = 204,

    // Indicates that a fetch ad request timed out (usually due to poor
    // connectivity).
    AdRequestNetworkTimeout = -1001,

    // Indicates that the device is not connected to internet (for instance if
    // user
    // is in Airplane mode). This returns the same code as
    // NSURLErrorNotConnectedToInternet.
    NotConnectedToInternet = -1009,

    // Indicates that an unspecified network issue occurred.
    AdRequestUnspecifiedError = -1,

    // Indicates that there has been a failure to render an ad on screen.
    UnableToRenderAd = -6,

    // Indicates that an attempt to cache a resource to the filesystem failed;
    // the
    // device may be out of space.
    UnableToPrecacheResources = -200,

    // Indicates that an attempt to cache an image resource to the filesystem
    // failed; the device may be out of space.
    UnableToPrecacheImageResources = -201,

    // Indicates that an attempt to cache a video resource to the filesystem
    // failed;
    // the device may be out of space.
    UnableToPrecacheVideoResources = -202,

    // Indicates that a AppLovin servers have returned an invalid response.
    InvalidResponse = -800,

    // Indicates that there was an error while attempting to render a native ad
    UnableToRenderNativeAd = -700,

    // Indicates that an unspecified network issue occurred.
    UnableToPreloadNativeAd = -701,

    // Indicates that the impression has already been tracked.
    NativeAdImpressionAlreadyTracked = -702,

    //
    // Rewarded Videos
    //

    // Indicates that the developer called for a rewarded video before one was
    // available.
    IncentiviziedAdNotPreloaded = -300,

    // Indicates that an unknown server-side error occurred.
    IncentivizedUnknownServerError = -400,

    // Indicates that a reward validation requested timed out (usually due to
    // poor
    // connectivity).
    IncentivizedValidationNetworkTimeout = -500,

    // Indicates that the user exited out of the rewarded ad early
    // You may or may not wish to grant a reward depending on your preference.
    IncentivizedUserClosedVideo = -600,

    // Indicates that a postback URL you attempted to dispatch was empty or nil.
    InvalidURL = -900
};

namespace {
// clang-format off
constexpr auto k__initialize               = "AppLovin_initialize";
constexpr auto k__setTestAdsEnabled        = "AppLovin_setTestAdsEnabled";
constexpr auto k__setVerboseLogging        = "AppLovin_setVerboseLogging";
constexpr auto k__setMuted                 = "AppLovin_setMuted";
constexpr auto k__hasInterstitialAd        = "AppLovin_hasInterstitialAd";
constexpr auto k__showInterstitialAd       = "AppLovin_showInterstitialAd";
constexpr auto k__loadRewardedVideo        = "AppLovin_loadRewardedVideo";
constexpr auto k__hasRewardedVideo         = "AppLovin_hasRewardedVideo";
constexpr auto k__showRewardedVideo        = "AppLovin_showRewardedVideo";
constexpr auto k__onInterstitialAdHidden   = "AppLovin_onInterstitialAdHidden";
constexpr auto k__onRewardedVideoFailed    = "AppLovin_onRewardedVideoFailed";
constexpr auto k__onRewardedVideoDisplayed = "AppLovin_onRewardedVideoDisplayed";
constexpr auto k__onRewardedVideoHidden    = "AppLovin_onRewardedVideoHidden";
constexpr auto k__onUserRewardVerified     = "AppLovin_onUserRewardVerified";
// clang-format on
} // namespace

Self::AppLovin()
    : Self(Logger::getSystemLogger()) {}

Self::AppLovin(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    verified_ = false;
    errored_ = false;
    rewardedVideo_ = nullptr;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialAdHidden();
            return "";
        },
        k__onInterstitialAdHidden);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewardedVideoFailed(std::stoi(message));
            return "";
        },
        k__onRewardedVideoFailed);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewardedVideoDisplayed();
            return "";
        },
        k__onRewardedVideoDisplayed);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewardedVideoHidden();
            return "";
        },
        k__onRewardedVideoHidden);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onUserRewardVerified();
            return "";
        },
        k__onUserRewardVerified);
}

Self::~AppLovin() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onInterstitialAdHidden);
    bridge_.deregisterHandler(k__onRewardedVideoFailed);
}

void Self::initialize(const std::string& key) {
    logger_.debug("%s: key = %s", __PRETTY_FUNCTION__, key.c_str());
    bridge_.call(k__initialize, key);
}

void Self::setTestAdsEnabled(bool enabled) {
    bridge_.call(k__setTestAdsEnabled, core::toString(enabled));
}

void Self::setVerboseLogging(bool enabled) {
    bridge_.call(k__setVerboseLogging, core::toString(enabled));
}

void Self::setMuted(bool enabled) {
    bridge_.call(k__setMuted, core::toString(enabled));
}

bool Self::hasInterstitialAd() const {
    auto result = bridge_.call(k__hasInterstitialAd);
    return core::toBool(result);
}

bool Self::showInterstitialAd() {
    if (not hasInterstitialAd()) {
        return false;
    }
    bridge_.call(k__showInterstitialAd);
    return true;
}

std::shared_ptr<IRewardedVideo> Self::createRewardedVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (rewardedVideo_ != nullptr) {
        return std::make_shared<NullRewardedVideo>(logger_);
    }
    auto result = new RewardedVideo(logger_, this);
    rewardedVideo_ = result;
    return std::shared_ptr<IRewardedVideo>(result);
}

bool Self::destroyRewardedVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (rewardedVideo_ == nullptr) {
        return false;
    }
    rewardedVideo_ = nullptr;
    return true;
}

void Self::loadRewardedVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.call(k__loadRewardedVideo);
}

bool Self::hasRewardedVideo() const {
    auto result = bridge_.call(k__hasRewardedVideo);
    return core::toBool(result);
}

bool Self::showRewardedVideo() {
    if (not hasRewardedVideo()) {
        return false;
    }
    verified_ = false;
    errored_ = false;
    bridge_.call(k__showRewardedVideo);

    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo(
        [this](bool rewarded) { this->rewardedVideo_->setResult(rewarded); });
    assert(successful);
    return not errored_;
}

void Self::onInterstitialAdHidden() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

void Self::onRewardedVideoFailed(int errorCode) {
    logger_.debug("%s: errorCode = %d", __PRETTY_FUNCTION__, errorCode);
    if (not errored_) {
        errored_ = true;
    }
}

void Self::onRewardedVideoDisplayed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

void Self::onRewardedVideoHidden() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    assert(rewardedVideo_ != nullptr);

    auto&& mediation = ads::MediationManager::getInstance();
    auto result = false;
    if (verified_ && not errored_) {
        result = true;
    }

    auto wasRewardedVideo = mediation.finishRewardedVideo(result);
    assert(wasRewardedVideo);
}

void Self::onUserRewardVerified() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    verified_ = true;
}
} // namespace applovin
} // namespace ee
