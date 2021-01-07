//
//  ALovinAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/app_lovin/private/AppLovinBridge.hpp"

#include <cassert>

#include <ee/ads/internal/GuardedFullScreenAd.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/SwitchToLibraryThread.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>

#include "ee/app_lovin/private/AppLovinRewardedAd.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IAppLovin>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    if (not addPlugin(Plugin::AppLovin)) {
        return nullptr;
    }
    return std::make_shared<app_lovin::Bridge>(bridge);
}
} // namespace core

namespace app_lovin {
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
const std::string kPrefix = "AppLovinBridge";

const auto kInitialize         = kPrefix + "Initialize";
const auto kSetVerboseLogging  = kPrefix + "SetVerboseLogging";
const auto kSetMuted           = kPrefix + "SetMuted";

const auto kHasInterstitialAd  = kPrefix + "HasInterstitialAd";
const auto kLoadInterstitialAd = kPrefix + "LoadInterstitialAd";
const auto kShowInterstitialAd = kPrefix + "ShowInterstitialAd";

const auto kHasRewardedAd      = kPrefix + "HasRewardedAd";
const auto kLoadRewardedAd     = kPrefix + "LoadRewardedAd";
const auto kShowRewardedAd     = kPrefix + "ShowRewardedAd";

const auto kOnInterstitialAdLoaded       = kPrefix + "OnInterstitialAdLoaded";
const auto kOnInterstitialAdFailedToLoad = kPrefix + "OnInterstitialAdFailedToLoad";
const auto kOnInterstitialAdClicked      = kPrefix + "OnInterstitialAdClicked";
const auto kOnInterstitialAdClosed       = kPrefix + "OnInterstitialAdClosed";

const auto kOnRewardedAdLoaded       = kPrefix + "OnRewardedAdLoaded";
const auto kOnRewardedAdFailedToLoad = kPrefix + "OnRewardedAdFailedToLoad";
const auto kOnRewardedAdClicked      = kPrefix + "OnRewardedAdClicked";
const auto kOnRewardedAdClosed       = kPrefix + "OnRewardedAdClosed";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge)
    , logger_(Logger::getSystemLogger()) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    displayer_ = mediation.getAdDisplayer();
    rewardedAd_ = nullptr;

    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this] { //
                onInterstitialAdLoaded();
            });
            return "";
        },
        kOnInterstitialAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] { //
                onInterstitialAdFailedToLoad(message);
            });
            return "";
        },
        kOnInterstitialAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this] { //
                onInterstitialAdClicked();
            });
            return "";
        },
        kOnInterstitialAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this] { //
                onInterstitialAdClosed();
            });
            return "";
        },
        kOnInterstitialAdClosed);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this] { //
                onRewardedAdLoaded();
            });
            return "";
        },
        kOnRewardedAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] { //
                onRewardedAdFailedToLoad(message);
            });
            return "";
        },
        kOnRewardedAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this] { //
                onRewardedAdClicked();
            });
            return "";
        },
        kOnRewardedAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] { //
                onRewardedAdClosed(core::toBool(message));
            });
            return "";
        },
        kOnRewardedAdClosed);
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.deregisterHandler(kOnInterstitialAdLoaded);
    bridge_.deregisterHandler(kOnInterstitialAdFailedToLoad);
    bridge_.deregisterHandler(kOnInterstitialAdClicked);
    bridge_.deregisterHandler(kOnInterstitialAdClosed);
    bridge_.deregisterHandler(kOnRewardedAdLoaded);
    bridge_.deregisterHandler(kOnRewardedAdFailedToLoad);
    bridge_.deregisterHandler(kOnRewardedAdClicked);
    bridge_.deregisterHandler(kOnRewardedAdClosed);

    PluginManager::removePlugin(Plugin::AppLovin);
}

Task<bool> Self::initialize(const std::string& key) {
    logger_.debug("%s: key = %s", __PRETTY_FUNCTION__, key.c_str());
    auto response = co_await bridge_.callAsync(kInitialize, key);
    co_await SwitchToLibraryThread();
    co_return core::toBool(response);
}

void Self::setVerboseLogging(bool enabled) {
    bridge_.call(kSetVerboseLogging, core::toString(enabled));
}

void Self::setMuted(bool enabled) {
    bridge_.call(kSetMuted, core::toString(enabled));
}

bool Self::hasInterstitialAd() const {
    auto result = bridge_.call(kHasInterstitialAd);
    return core::toBool(result);
}

void Self::loadInterstitialAd() {
    bridge_.call(kLoadInterstitialAd);
}

void Self::showInterstitialAd() {
    bridge_.call(kShowInterstitialAd);
}

std::shared_ptr<IFullScreenAd> Self::createRewardedAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (sharedRewardedAd_) {
        return sharedRewardedAd_;
    }
    rewardedAd_ = new RewardedAd(logger_, displayer_, this);
    sharedRewardedAd_ = std::make_shared<ads::GuardedFullScreenAd>(
        std::shared_ptr<RewardedAd>(rewardedAd_));
    return sharedRewardedAd_;
}

bool Self::destroyRewardedAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (sharedRewardedAd_ == nullptr) {
        return false;
    }
    rewardedAd_ = nullptr;
    sharedRewardedAd_.reset();
    return true;
}

bool Self::hasRewardedAd() const {
    auto result = bridge_.call(kHasRewardedAd);
    return core::toBool(result);
}

void Self::loadRewardedAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.call(kLoadRewardedAd);
}

void Self::showRewardedAd() {
    bridge_.call(kShowRewardedAd);
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
    if (rewardedAd_) {
        rewardedAd_->onLoaded();
    } else {
        assert(false);
    }
}

void Self::onRewardedAdFailedToLoad(const std::string& message) {
    logger_.debug("%s: message = %s", __PRETTY_FUNCTION__, message.c_str());
    if (rewardedAd_) {
        rewardedAd_->onFailedToLoad(message);
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClicked() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (rewardedAd_) {
        rewardedAd_->onClicked();
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClosed(bool rewarded) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (rewardedAd_) {
        rewardedAd_->onClosed(rewarded);
    } else {
        assert(false);
    }
}
} // namespace app_lovin
} // namespace ee
