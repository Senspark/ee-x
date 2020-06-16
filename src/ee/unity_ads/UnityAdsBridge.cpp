//
//  UnityAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/unity_ads/UnityAdsBridge.hpp"

#include <cassert>

#include <ee/nlohmann/json.hpp>

#include <ee/ads/internal/GuardedInterstitialAd.hpp>
#include <ee/ads/internal/GuardedRewardedAd.hpp>
#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/unity_ads/private/UnityInterstitialAd.hpp"
#include "ee/unity_ads/private/UnityRewardedAd.hpp"

namespace ee {
namespace unity_ads {
using Self = Bridge;

namespace {
// clang-format off
const std::string kPrefix         = "UnityAds";

const auto k__initialize          = kPrefix + "_initialize";
const auto k__setDebugModeEnabled = kPrefix + "_setDebugModeEnabled";
const auto k__hasRewardedAd       = kPrefix + "_hasRewardedAd";
const auto k__showRewardedAd      = kPrefix + "_showRewardedAd";
const auto k__onLoaded            = kPrefix + "_onLoaded";
const auto k__onFailedToShow      = kPrefix + "_onFailedToShow";
const auto k__onClosed            = kPrefix + "_onClosed";
// clang-format on
} // namespace

namespace {
// clang-format off
constexpr auto k__gameId          = "gameId";
constexpr auto k__testModeEnabled = "testModeEnabled";
// clang-format on
} // namespace

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    PluginManager::addPlugin(Plugin::UnityAds);

    displaying_ = false;

    auto&& mediation = ads::MediationManager::getInstance();
    interstitialAdDisplayer_ = mediation.getInterstitialAdDisplayer();
    rewardedAdDisplayer_ = mediation.getRewardedAdDisplayer();

    bridge_.registerHandler(
        [this](const std::string& message) {
            onLoaded(message);
            return "";
        },
        k__onLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToShow(json["ad_id"], json["message"]);
            return "";
        },
        k__onFailedToShow);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onClosed(json["ad_id"], json["rewarded"]);
            return "";
        },
        k__onClosed);
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug(__PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onLoaded);
    bridge_.deregisterHandler(k__onFailedToShow);
    bridge_.deregisterHandler(k__onClosed);
    PluginManager::removePlugin(Plugin::UnityAds);
}

void Self::initialize(const std::string& gameId, bool testModeEnabled) {
    logger_.debug("%s: gameId = %s test = %s", __PRETTY_FUNCTION__,
                  gameId.c_str(), core::toString(testModeEnabled).c_str());
    runOnUiThread([this, gameId, testModeEnabled] {
        nlohmann::json json;
        json[k__gameId] = gameId;
        json[k__testModeEnabled] = testModeEnabled;
        bridge_.call(k__initialize, json.dump());
    });
}

void Self::setDebugModeEnabled(bool enabled) {
    runOnUiThread([this, enabled] {
        bridge_.call(k__setDebugModeEnabled, core::toString(enabled));
    });
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = interstitialAds_.find(adId);
    if (iter != interstitialAds_.cend()) {
        return iter->second.strong;
    }
    auto weak =
        new InterstitialAd(logger_, interstitialAdDisplayer_, this, adId);
    auto strong = std::make_shared<ads::GuardedInterstitialAd>(
        std::shared_ptr<InterstitialAd>(weak));
    interstitialAds_.try_emplace(adId, strong, weak);
    return strong;
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
        return iter->second.strong;
    }
    auto weak = new RewardedAd(logger_, rewardedAdDisplayer_, this, adId);
    auto strong = std::make_shared<ads::GuardedRewardedAd>(
        std::shared_ptr<RewardedAd>(weak));
    rewardedAds_.try_emplace(adId, strong, weak);
    return strong;
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
    assert(isMainThread());
    auto response = bridge_.call(k__hasRewardedAd, adId);
    return core::toBool(response);
}

void Self::showRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    adId_ = adId;
    displaying_ = true;
    runOnUiThread([this, adId] { //
        bridge_.call(k__showRewardedAd, adId);
    });
}

void Self::onLoaded(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (auto iter = interstitialAds_.find(adId);
        iter != interstitialAds_.cend()) {
        iter->second.weak->onLoaded();
        return;
    }
    if (auto iter = rewardedAds_.find(adId); iter != rewardedAds_.cend()) {
        iter->second.weak->onLoaded();
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
            iter->second.weak->onFailedToShow(message);
        }
        if (auto iter = rewardedAds_.find(adId); iter != rewardedAds_.cend()) {
            iter->second.weak->onFailedToShow(message);
        }
        displaying_ = false;
    } else {
        // Mediation.
        assert(false);
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
            iter->second.weak->onClosed();
        }
        if (auto iter = rewardedAds_.find(adId); iter != rewardedAds_.cend()) {
            iter->second.weak->onClosed(rewarded);
        }
    } else {
        // Mediation.
        assert(false);
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
