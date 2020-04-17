#include "ee/iron_source/IronSourceBridge.hpp"

#include <cassert>

#include <ee/nlohmann/json.hpp>

#include <ee/ads/NullInterstitialAd.hpp>
#include <ee/ads/NullRewardedAd.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/core/internal/SharedPtrUtils.hpp>

#include "ee/iron_source/private/IronSourceInterstitialAd.hpp"
#include "ee/iron_source/private/IronSourceRewardedAd.hpp"

namespace ee {
namespace iron_source {
using Self = Bridge;

namespace {
// clang-format off
const std::string kPrefix                  = "IronSource";

const auto k__initialize                   = kPrefix + "_initialize";

const auto k__hasInterstitialAd            = kPrefix + "_hasInterstitialAd";
const auto k__loadInterstitialAd           = kPrefix + "_loadInterstitialAd";
const auto k__showInterstitialAd           = kPrefix + "_showInterstitialAd";

const auto k__hasRewardedAd                = kPrefix + "_hasRewardedAd";
const auto k__showRewardedAd               = kPrefix + "_showRewardedAd";

const auto k__onInterstitialAdLoaded       = kPrefix + "_onInterstitialAdLoaded";
const auto k__onInterstitialAdFailedToLoad = kPrefix + "_onInterstitialAdFailedToLoad";
const auto k__onInterstitialAdClicked      = kPrefix + "_onInterstitialAdClicked";
const auto k__onInterstitialAdClosed       = kPrefix + "_onInterstitialAdClosed";

const auto k__onRewardedAdClicked = kPrefix + "_onRewardedAdClicked";
const auto k__onRewardedAdClosed  = kPrefix + "_onRewardedAdClosed";
// clang-format on
} // namespace

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);

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

    bridge_.deregisterHandler(k__onRewardedAdClicked);
    bridge_.deregisterHandler(k__onRewardedAdClosed);
}

void Self::initialize(const std::string& gameId) {
    logger_.debug("%s: gameId = %s", __PRETTY_FUNCTION__, gameId.c_str());
    bridge_.call(k__initialize, gameId);
}

void Self::setCloseTimeOut(float timeOut) {
    // Already managed in iOS/Android.
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (not interstitialAd_.expired()) {
        return interstitialAd_.lock();
    }
    auto result = std::shared_ptr<InterstitialAd>(
        new InterstitialAd(logger_, this, adId));
    interstitialAd_ = result;
    return result;
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (interstitialAd_.expired()) {
        return false;
    }
    return true;
}

std::shared_ptr<IRewardedAd> Self::createRewardedAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the smae instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (not rewardedAd_.expired()) {
        return rewardedAd_.lock();
    }
    auto result =
        std::shared_ptr<RewardedAd>(new RewardedAd(logger_, this, adId));
    rewardedAd_ = result;
    return result;
}

bool Self::destroyRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (rewardedAd_.expired()) {
        return false;
    }
    return true;
}

bool Self::hasInterstitialAd() const {
    auto response = bridge_.call(k__hasInterstitialAd);
    return core::toBool(response);
}

void Self::loadInterstitialAd() {
    bridge_.call(k__loadInterstitialAd);
}

void Self::showInterstitialAd(const std::string& adId) {
    bridge_.call(k__showInterstitialAd, adId);
}

bool Self::hasRewardedAd() const {
    auto response = bridge_.call(k__hasRewardedAd);
    return core::toBool(response);
}

void Self::showRewardedAd(const std::string& adId) {
    bridge_.call(k__showRewardedAd, adId);
}

#pragma mark - Interstitial Ad Callbacks.

void Self::onInterstitialAdLoaded() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto ad = interstitialAd_.lock();
    if (ad) {
        ad->onLoaded();
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdFailedToLoad(const std::string& message) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto ad = interstitialAd_.lock();
    if (ad) {
        ad->onFailedToLoad(message);
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdClicked() {
    auto ad = interstitialAd_.lock();
    if (ad) {
        ad->onClicked();
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto ad = interstitialAd_.lock();
    if (ad) {
        ad->onClosed();
    } else {
        assert(false);
        onMediationAdClosed(false);
    }
}

#pragma mark - Rewarded Ad Callbacks.

void Self::onRewardedAdFailedToShow(const std::string& message) {
    auto ad = rewardedAd_.lock();
    if (ad) {
        ad->onFailedToShow(message);
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClicked() {
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
        onMediationAdClosed(rewarded);
    }
}

#pragma mark - Mediation Ad Callbacks.

void Self::onMediationAdClosed(bool rewarded) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();

    // Other mediation network.
    auto wasInterstitialAd = mediation.setInterstitialAdDone();
    auto wasRewardedVideo = mediation.finishRewardedVideo(false);

    assert(wasInterstitialAd || wasRewardedVideo);
}
} // namespace iron_source
} // namespace ee
