#include "ee/iron_source/private/IronSourceBridge.hpp"

#include <ee/ads/internal/GuardedInterstitialAd.hpp>
#include <ee/ads/internal/GuardedRewardedAd.hpp>
#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/iron_source/private/IronSourceInterstitialAd.hpp"
#include "ee/iron_source/private/IronSourceRewardedAd.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IIronSource>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    if (not addPlugin(Plugin::IronSource)) {
        return nullptr;
    }
    return std::make_shared<iron_source::Bridge>(bridge);
}
} // namespace core

namespace iron_source {
namespace {
// clang-format off
const std::string kPrefix                = "IronSourceBridge";

const auto kInitialize                   = kPrefix + "Initialize";

const auto kHasInterstitialAd            = kPrefix + "HasInterstitialAd";
const auto kLoadInterstitialAd           = kPrefix + "LoadInterstitialAd";
const auto kShowInterstitialAd           = kPrefix + "ShowInterstitialAd";

const auto kHasRewardedAd                = kPrefix + "HasRewardedAd";
const auto kShowRewardedAd               = kPrefix + "ShowRewardedAd";

const auto kOnInterstitialAdLoaded       = kPrefix + "OnInterstitialAdLoaded";
const auto kOnInterstitialAdFailedToLoad = kPrefix + "OnInterstitialAdFailedToLoad";
const auto kOnInterstitialAdFailedToShow = kPrefix + "OnInterstitialAdFailedToShow";
const auto kOnInterstitialAdClicked      = kPrefix + "OnInterstitialAdClicked";
const auto kOnInterstitialAdClosed       = kPrefix + "OnInterstitialAdClosed";

const auto kOnRewardedAdLoaded       = kPrefix + "OnRewardedAdLoaded";
const auto kOnRewardedAdFailedToShow = kPrefix + "OnRewardedAdFailedToShow";
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
    interstitialAdDisplayer_ = mediation.getInterstitialAdDisplayer();
    rewardedAdDisplayer_ = mediation.getRewardedAdDisplayer();
    interstitialAd_ = nullptr;
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
            Thread::runOnLibraryThread([this, message] { //
                onInterstitialAdFailedToShow(message);
            });
            return "";
        },
        kOnInterstitialAdFailedToShow);
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
                onRewardedAdFailedToShow(message);
            });
            return "";
        },
        kOnRewardedAdFailedToShow);
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
    bridge_.deregisterHandler(kOnRewardedAdFailedToShow);
    bridge_.deregisterHandler(kOnInterstitialAdClicked);
    bridge_.deregisterHandler(kOnInterstitialAdClosed);

    bridge_.deregisterHandler(kOnRewardedAdLoaded);
    bridge_.deregisterHandler(kOnRewardedAdFailedToShow);
    bridge_.deregisterHandler(kOnRewardedAdClicked);
    bridge_.deregisterHandler(kOnRewardedAdClosed);

    PluginManager::removePlugin(Plugin::IronSource);
}

Task<bool> Self::initialize(const std::string& appKey) {
    logger_.debug("%s: appKey = %s", __PRETTY_FUNCTION__, appKey.c_str());
    auto response = co_await bridge_.callAsync(kInitialize, appKey);
    co_return core::toBool(response);
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedInterstitialAd_) {
        return sharedInterstitialAd_;
    }
    interstitialAd_ =
        new InterstitialAd(logger_, interstitialAdDisplayer_, this, adId);
    sharedInterstitialAd_ = std::make_shared<ads::GuardedInterstitialAd>(
        std::shared_ptr<InterstitialAd>(interstitialAd_));
    return sharedInterstitialAd_;
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedInterstitialAd_ == nullptr) {
        return false;
    }
    interstitialAd_ = nullptr;
    sharedInterstitialAd_.reset();
    return true;
}

std::shared_ptr<IRewardedAd> Self::createRewardedAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedRewardedAd_) {
        return sharedRewardedAd_;
    }
    rewardedAd_ = new RewardedAd(logger_, rewardedAdDisplayer_, this, adId);
    sharedRewardedAd_ = std::make_shared<ads::GuardedRewardedAd>(
        std::shared_ptr<RewardedAd>(rewardedAd_));
    return sharedRewardedAd_;
}

bool Self::destroyRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedRewardedAd_ == nullptr) {
        return false;
    }
    rewardedAd_ = nullptr;
    sharedRewardedAd_.reset();
    return true;
}

bool Self::hasInterstitialAd() const {
    auto response = bridge_.call(kHasInterstitialAd);
    return core::toBool(response);
}

void Self::loadInterstitialAd() {
    bridge_.call(kLoadInterstitialAd);
}

void Self::showInterstitialAd(const std::string& adId) {
    bridge_.call(kShowInterstitialAd, adId);
}

bool Self::hasRewardedAd() const {
    auto response = bridge_.call(kHasRewardedAd);
    return core::toBool(response);
}

void Self::showRewardedAd(const std::string& adId) {
    bridge_.call(kShowRewardedAd, adId);
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
