#include "ee/iron_source/IronSourceBridge.hpp"

#include <cassert>

#include <ee/nlohmann/json.hpp>

#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

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
const auto k__onInterstitialAdFailedToShow = kPrefix + "_onInterstitialAdFailedToShow";
const auto k__onInterstitialAdClicked      = kPrefix + "_onInterstitialAdClicked";
const auto k__onInterstitialAdClosed       = kPrefix + "_onInterstitialAdClosed";

const auto k__onRewardedAdLoaded       = kPrefix + "_onRewardedAdLoaded";
const auto k__onRewardedAdFailedToShow = kPrefix + "_onRewardedAdFailedToShow";
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
    interstitialAdDisplayer_ = mediation.getInterstitialAdDisplayer();
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
            onInterstitialAdFailedToShow(message);
            return "";
        },
        k__onInterstitialAdFailedToShow);
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
            onRewardedAdFailedToShow(message);
            return "";
        },
        k__onRewardedAdFailedToShow);
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

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.deregisterHandler(k__onInterstitialAdLoaded);
    bridge_.deregisterHandler(k__onInterstitialAdFailedToLoad);
    bridge_.deregisterHandler(k__onRewardedAdFailedToShow);
    bridge_.deregisterHandler(k__onInterstitialAdClicked);
    bridge_.deregisterHandler(k__onInterstitialAdClosed);

    bridge_.deregisterHandler(k__onRewardedAdLoaded);
    bridge_.deregisterHandler(k__onRewardedAdFailedToShow);
    bridge_.deregisterHandler(k__onRewardedAdClicked);
    bridge_.deregisterHandler(k__onRewardedAdClosed);
}

void Self::initialize(const std::string& gameId) {
    logger_.debug("%s: gameId = %s", __PRETTY_FUNCTION__, gameId.c_str());
    runOnUiThread([this, gameId] { //
        bridge_.call(k__initialize, gameId);
    });
}

void Self::setCloseTimeOut(float timeOut) {
    // Already managed in iOS/Android.
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (interstitialAd_) {
        return interstitialAd_;
    }
    auto ad = std::shared_ptr<InterstitialAd>(
        new InterstitialAd(logger_, interstitialAdDisplayer_, this, adId));
    interstitialAd_ = ad;
    return ad;
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (interstitialAd_ == nullptr) {
        return false;
    }
    interstitialAd_.reset();
    return true;
}

std::shared_ptr<IRewardedAd> Self::createRewardedAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (rewardedAd_) {
        return rewardedAd_;
    }
    auto ad = std::shared_ptr<RewardedAd>(
        new RewardedAd(logger_, rewardedAdDisplayer_, this, adId));
    rewardedAd_ = ad;
    return ad;
}

bool Self::destroyRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (rewardedAd_ == nullptr) {
        return false;
    }
    rewardedAd_.reset();
    return true;
}

bool Self::hasInterstitialAd() const {
    assert(isMainThread());
    auto response = bridge_.call(k__hasInterstitialAd);
    return core::toBool(response);
}

void Self::loadInterstitialAd() {
    runOnUiThread([this] { //
        bridge_.call(k__loadInterstitialAd);
    });
}

void Self::showInterstitialAd(const std::string& adId) {
    runOnUiThread([this, adId] { //
        bridge_.call(k__showInterstitialAd, adId);
    });
}

bool Self::hasRewardedAd() const {
    assert(isMainThread());
    auto response = bridge_.call(k__hasRewardedAd);
    return core::toBool(response);
}

void Self::showRewardedAd(const std::string& adId) {
    runOnUiThread([this, adId] { //
        bridge_.call(k__showRewardedAd, adId);
    });
}

#pragma mark - Interstitial Ad Callbacks.

void Self::onInterstitialAdLoaded() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAd_) {
        interstitialAd_->onLoaded();
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdFailedToLoad(const std::string& message) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAd_) {
        interstitialAd_->onFailedToLoad(message);
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdFailedToShow(const std::string& message) {
    if (interstitialAd_) {
        interstitialAd_->onFailedToShow(message);
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdClicked() {
    if (interstitialAd_) {
        interstitialAd_->onClicked();
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAd_) {
        interstitialAd_->onClosed();
    } else {
        assert(false);
        onMediationAdClosed(false);
    }
}

#pragma mark - Rewarded Ad Callbacks.

void Self::onRewardedAdLoaded() {
    if (rewardedAd_) {
        rewardedAd_->onLoaded();
    } else {
        assert(false);
    }
}

void Self::onRewardedAdFailedToShow(const std::string& message) {
    if (rewardedAd_) {
        rewardedAd_->onFailedToShow(message);
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClicked() {
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
        onMediationAdClosed(rewarded);
    }
}

#pragma mark - Mediation Ad Callbacks.

void Self::onMediationAdClosed(bool rewarded) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAdDisplayer_->isProcessing()) {
        interstitialAdDisplayer_->resolve(true);
        return;
    }
    if (rewardedAdDisplayer_->isProcessing()) {
        rewardedAdDisplayer_->resolve(rewarded ? IRewardedAdResult::Completed
                                               : IRewardedAdResult::Canceled);
        return;
    }
    assert(false);
}
} // namespace iron_source
} // namespace ee
