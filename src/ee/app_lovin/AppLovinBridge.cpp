//
//  ALovinAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/app_lovin/AppLovinBridge.hpp"

#include <cassert>

#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/app_lovin/private/AppLovinRewardedAd.hpp"

namespace ee {
namespace app_lovin {
using Self = Bridge;

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
const std::string kPrefix        = "AppLovin";

const auto k__initialize         = kPrefix + "_initialize";
const auto k__setTestAdsEnabled  = kPrefix + "_setTestAdsEnabled";
const auto k__setVerboseLogging  = kPrefix + "_setVerboseLogging";
const auto k__setMuted           = kPrefix + "_setMuted";

const auto k__hasInterstitialAd  = kPrefix + "_hasInterstitialAd";
const auto k__loadInterstitialAd = kPrefix + "_loadInterstitialAd";
const auto k__showInterstitialAd = kPrefix + "_showInterstitialAd";

const auto k__hasRewardedAd      = kPrefix + "_hasRewardedAd";
const auto k__loadRewardedAd     = kPrefix + "_loadRewardedAd";
const auto k__showRewardedAd     = kPrefix + "_showRewardedAd";

const auto k__onInterstitialAdLoaded       = kPrefix + "_onInterstitialAdLoaded";
const auto k__onInterstitialAdFailedToLoad = kPrefix + "_onInterstitialAdFailedToLoad";
const auto k__onInterstitialAdClicked      = kPrefix + "_onInterstitialAdClicked";
const auto k__onInterstitialAdClosed       = kPrefix + "_onInterstitialAdClosed";

const auto k__onRewardedAdLoaded       = kPrefix + "_onRewardedAdLoaded";
const auto k__onRewardedAdFailedToLoad = kPrefix + "_onRewardedAdFailedToLoad";
const auto k__onRewardedAdClicked      = kPrefix + "_onRewardedAdClicked";
const auto k__onRewardedAdClosed       = kPrefix + "_onRewardedAdClosed";
// clang-format on
} // namespace

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    rewardedAdDisplayer_ = mediation.getRewardedAdDisplayer();

    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialAdLoaded();
            return "";
        },
        k__onInterstitialAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialAdFailedToLoad(message);
            return "";
        },
        k__onInterstitialAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialAdClicked();
            return "";
        },
        k__onInterstitialAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onInterstitialAdClosed();
            return "";
        },
        k__onInterstitialAdClosed);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewardedAdLoaded();
            return "";
        },
        k__onRewardedAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewardedAdFailedToLoad(message);
            return "";
        },
        k__onRewardedAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewardedAdClicked();
            return "";
        },
        k__onRewardedAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onRewardedAdClosed(core::toBool(message));
            return "";
        },
        k__onRewardedAdClosed);
}

Self::~Bridge() {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.deregisterHandler(k__onInterstitialAdLoaded);
    bridge_.deregisterHandler(k__onInterstitialAdFailedToLoad);
    bridge_.deregisterHandler(k__onInterstitialAdClicked);
    bridge_.deregisterHandler(k__onInterstitialAdClosed);
    bridge_.deregisterHandler(k__onRewardedAdLoaded);
    bridge_.deregisterHandler(k__onRewardedAdFailedToLoad);
    bridge_.deregisterHandler(k__onRewardedAdClicked);
    bridge_.deregisterHandler(k__onRewardedAdClosed);
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

void Self::loadInterstitialAd() {
    bridge_.call(k__loadInterstitialAd);
}

void Self::showInterstitialAd() {
    bridge_.call(k__showInterstitialAd);
}

std::shared_ptr<IRewardedAd> Self::createRewardedAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (not rewardedAd_.expired()) {
        return rewardedAd_.lock();
    }
    auto result =
        std::shared_ptr<RewardedAd>(new RewardedAd(rewardedAdDisplayer_, this));
    rewardedAd_ = result;
    return result;
}

bool Self::destroyRewardedAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (rewardedAd_.expired()) {
        return false;
    }
    return true;
}

bool Self::hasRewardedAd() const {
    auto result = bridge_.call(k__hasRewardedAd);
    return core::toBool(result);
}

void Self::loadRewardedAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.call(k__loadRewardedAd);
}

void Self::showRewardedAd() {
    bridge_.call(k__showRewardedAd);
}

void Self::onInterstitialAdLoaded() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    // TODO.
}

void Self::onInterstitialAdFailedToLoad(const std::string& message) {
    logger_.debug("%s: message = %s", __PRETTY_FUNCTION__, message.c_str());
    // TODO.
}

void Self::onInterstitialAdClicked() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    // TODO.
}

void Self::onInterstitialAdClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    // TODO.
}

void Self::onRewardedAdLoaded() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto ad = rewardedAd_.lock();
    if (ad) {
        ad->onLoaded();
    } else {
        assert(false);
    }
}

void Self::onRewardedAdFailedToLoad(const std::string& message) {
    logger_.debug("%s: message = %s", __PRETTY_FUNCTION__, message.c_str());
    auto ad = rewardedAd_.lock();
    if (ad) {
        ad->onFailedToLoad(message);
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClicked() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto ad = rewardedAd_.lock();
    if (ad) {
        ad->onClicked();
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClosed(bool rewarded) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto ad = rewardedAd_.lock();
    if (ad) {
        ad->onClosed(rewarded);
    } else {
        assert(false);
    }
}
} // namespace app_lovin
} // namespace ee
