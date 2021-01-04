//
//  UnityAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/unity_ads/private/UnityAdsBridge.hpp"

#include <ee/ads/internal/GuardedInterstitialAd.hpp>
#include <ee/ads/internal/GuardedRewardedAd.hpp>
#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/SwitchToLibraryThread.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/unity_ads/private/UnityInterstitialAd.hpp"
#include "ee/unity_ads/private/UnityRewardedAd.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IUnityAds>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    if (not addPlugin(Plugin::UnityAds)) {
        return nullptr;
    }
    return std::make_shared<unity_ads::Bridge>(bridge);
}
} // namespace core

namespace unity_ads {
namespace {
// clang-format off
const std::string kPrefix       = "UnityAdsBridge";
const auto kInitialize          = kPrefix + "Initialize";
const auto kSetDebugModeEnabled = kPrefix + "SetDebugModeEnabled";
const auto kHasRewardedAd       = kPrefix + "HasRewardedAd";
const auto kLoadRewardedAd      = kPrefix + "LoadRewardedAd";
const auto kShowRewardedAd      = kPrefix + "ShowRewardedAd";
const auto kOnLoaded            = kPrefix + "OnLoaded";
const auto kOnFailedToShow      = kPrefix + "OnFailedToShow";
const auto kOnClosed            = kPrefix + "OnClosed";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge)
    , logger_(Logger::getSystemLogger()) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    displaying_ = false;

    auto&& mediation = ads::MediationManager::getInstance();
    interstitialAdDisplayer_ = mediation.getInterstitialAdDisplayer();
    rewardedAdDisplayer_ = mediation.getRewardedAdDisplayer();

    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] { //
                onLoaded(message);
            });
            return "";
        },
        kOnLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] { //
                auto json = nlohmann::json::parse(message);
                onFailedToShow(json["ad_id"], json["message"]);
            });
            return "";
        },
        kOnFailedToShow);
    bridge_.registerHandler(
        [this](const std::string& message) {
            Thread::runOnLibraryThread([this, message] { //
                auto json = nlohmann::json::parse(message);
                onClosed(json["ad_id"], json["rewarded"]);
            });
            return "";
        },
        kOnClosed);
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug(__PRETTY_FUNCTION__);
    bridge_.deregisterHandler(kOnLoaded);
    bridge_.deregisterHandler(kOnFailedToShow);
    bridge_.deregisterHandler(kOnClosed);
    PluginManager::removePlugin(Plugin::UnityAds);
}

Task<bool> Self::initialize(const std::string& gameId, bool testModeEnabled) {
    logger_.debug("%s: gameId = %s test = %s", __PRETTY_FUNCTION__,
                  gameId.c_str(), core::toString(testModeEnabled).c_str());
    nlohmann::json json;
    json["gameId"] = gameId;
    json["testModeEnabled"] = testModeEnabled;
    auto response = co_await bridge_.callAsync(kInitialize, json.dump());
    co_await SwitchToLibraryThread();
    co_return core::toBool(response);
}

void Self::setDebugModeEnabled(bool enabled) {
    bridge_.call(kSetDebugModeEnabled, core::toString(enabled));
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = interstitialAds_.find(adId);
    if (iter != interstitialAds_.cend()) {
        return iter->second.ad;
    }
    auto raw = std::make_shared<InterstitialAd>(
        logger_, interstitialAdDisplayer_, this, adId);
    auto ad = std::make_shared<ads::GuardedInterstitialAd>(raw);
    interstitialAds_.try_emplace(adId, ad, raw);
    return ad;
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = interstitialAds_.find(adId);
    if (iter == interstitialAds_.cend()) {
        return false;
    }
    interstitialAds_.erase(iter);
    return true;
}

std::shared_ptr<IRewardedAd> Self::createRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        return iter->second.ad;
    }
    auto raw =
        std::make_shared<RewardedAd>(logger_, rewardedAdDisplayer_, this, adId);
    auto ad = std::make_shared<ads::GuardedRewardedAd>(raw);
    rewardedAds_.try_emplace(adId, ad, raw);
    return ad;
}

bool Self::destroyRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = rewardedAds_.find(adId);
    if (iter == rewardedAds_.cend()) {
        return false;
    }
    rewardedAds_.erase(iter);
    return true;
}

bool Self::hasRewardedAd(const std::string& adId) const {
    auto response = bridge_.call(kHasRewardedAd, adId);
    return core::toBool(response);
}

Task<bool> Self::loadRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto response = co_await bridge_.callAsync(kLoadRewardedAd, adId);
    co_await SwitchToLibraryThread();
    co_return core::toBool(response);
}

void Self::showRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    adId_ = adId;
    displaying_ = true;
    bridge_.call(kShowRewardedAd, adId);
}

void Self::onLoaded(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (auto iter = interstitialAds_.find(adId);
        iter != interstitialAds_.cend()) {
        iter->second.raw->onLoaded();
        return;
    }
    if (auto iter = rewardedAds_.find(adId); iter != rewardedAds_.cend()) {
        iter->second.raw->onLoaded();
        return;
    }
    // Mediation.
    logger_.error("%s: unexpected adId = %s", __PRETTY_FUNCTION__,
                  adId.c_str());
}

void Self::onFailedToShow(const std::string& adId, const std::string& message) {
    logger_.debug("%s: adId = %s message = %s", __PRETTY_FUNCTION__,
                  adId.c_str(), message.c_str());
    if (displaying_) {
        assert(adId_ == adId);
        if (auto iter = interstitialAds_.find(adId);
            iter != interstitialAds_.cend()) {
            iter->second.raw->onFailedToShow(message);
        }
        if (auto iter = rewardedAds_.find(adId); iter != rewardedAds_.cend()) {
            iter->second.raw->onFailedToShow(message);
        }
        displaying_ = false;
    } else {
        // Mediation.
        onMediationAdFailedToShow(adId, message);
    }
}

void Self::onClosed(const std::string& adId, bool rewarded) {
    logger_.debug("%s: adId = %s rewarded = %s", __PRETTY_FUNCTION__,
                  adId.c_str(), core::toString(rewarded).c_str());
    if (displaying_) {
        assert(adId_ == adId);
        if (auto iter = interstitialAds_.find(adId);
            iter != interstitialAds_.cend()) {
            iter->second.raw->onClosed();
        }
        if (auto iter = rewardedAds_.find(adId); iter != rewardedAds_.cend()) {
            iter->second.raw->onClosed(rewarded);
        }
    } else {
        // Mediation.
        onMediationAdClosed(adId, rewarded);
    }
}

void Self::onMediationAdFailedToShow(const std::string& adId,
                                     const std::string& message) {
    if (interstitialAdDisplayer_->isProcessing()) {
        interstitialAdDisplayer_->resolve(false);
        return;
    }
    if (rewardedAdDisplayer_->isProcessing()) {
        rewardedAdDisplayer_->resolve(IRewardedAdResult::Failed);
        return;
    }
    assert(false);
}

void Self::onMediationAdClosed(const std::string& adId, bool rewarded) {
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
} // namespace unity_ads
} // namespace ee
