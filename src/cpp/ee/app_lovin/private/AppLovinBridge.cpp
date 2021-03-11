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
#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/app_lovin/private/AppLovinRewardedAd.hpp"

namespace ee {
namespace app_lovin {
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

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer)
    , network_("app_lovin") {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    displayer_ = mediation.getAdDisplayer();
    rewardedAd_ = nullptr;

    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdLoaded();
        },
        kOnInterstitialAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onInterstitialAdFailedToLoad(json["code"], json["message"]);
        },
        kOnInterstitialAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdClicked();
        },
        kOnInterstitialAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdClosed();
        },
        kOnInterstitialAdClosed);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdLoaded();
        },
        kOnRewardedAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onRewardedAdFailedToLoad(json["code"], json["message"]);
        },
        kOnRewardedAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdClicked();
        },
        kOnRewardedAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdClosed(core::toBool(message));
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
    destroyer_();
}

Task<bool> Self::initialize(const std::string& key) {
    logger_.debug("%s: key = %s", __PRETTY_FUNCTION__, key.c_str());
    auto response = co_await bridge_.callAsync(kInitialize, key);
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
    rewardedAd_ = new RewardedAd(logger_, displayer_, this, network_);
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

void Self::onInterstitialAdFailedToLoad(int code, const std::string& message) {
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

void Self::onRewardedAdFailedToLoad(int code, const std::string& message) {
    logger_.debug("%s: message = %s", __PRETTY_FUNCTION__, message.c_str());
    if (rewardedAd_) {
        rewardedAd_->onFailedToLoad(code, message);
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
