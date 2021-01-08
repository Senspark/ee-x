//
//  UnityAds.cpp
//  ee_x
//
//  Created by Pham Xuan Han on 5/12/17.
//
//

#include "ee/unity_ads/private/UnityAdsBridge.hpp"

#include <ee/ads/internal/GuardedFullScreenAd.hpp>
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
    addPlugin(Plugin::UnityAds);
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
    displayer_ = mediation.getAdDisplayer();

    bridge_.registerHandler(
        [this](const std::string& message) { //
            onLoaded(message);
        },
        kOnLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToShow(json["ad_id"], json["message"]);
        },
        kOnFailedToShow);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onClosed(json["ad_id"], json["rewarded"]);
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

std::shared_ptr<IFullScreenAd>
Self::createInterstitialAd(const std::string& adId) {
    return createFullScreenAd<InterstitialAd>(adId);
}

std::shared_ptr<IFullScreenAd> Self::createRewardedAd(const std::string& adId) {
    return createFullScreenAd<RewardedAd>(adId);
}

template <class Ad>
std::shared_ptr<IFullScreenAd>
Self::createFullScreenAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter != ads_.cend()) {
        return std::dynamic_pointer_cast<IFullScreenAd>(iter->second.first);
    }
    auto raw = std::make_shared<Ad>(logger_, displayer_, this, adId);
    auto ad = std::make_shared<ads::GuardedFullScreenAd>(raw);
    ads_.try_emplace(adId, ad, raw);
    return ad;
}

bool Self::destroyAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter == ads_.cend()) {
        return false;
    }
    ads_.erase(iter);
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
    if (auto iter = ads_.find(adId); iter != ads_.cend()) {
        auto&& ad = iter->second.second;
        if (auto item = std::dynamic_pointer_cast<InterstitialAd>(ad);
            item != nullptr) {
            item->onLoaded();
            return;
        }
        if (auto item = std::dynamic_pointer_cast<RewardedAd>(ad);
            item != nullptr) {
            item->onLoaded();
            return;
        }
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
        displaying_ = false;
        if (auto iter = ads_.find(adId); iter != ads_.cend()) {
            auto&& ad = iter->second.second;
            if (auto item = std::dynamic_pointer_cast<InterstitialAd>(ad);
                item != nullptr) {
                item->onFailedToShow(message);
                return;
            }
            if (auto item = std::dynamic_pointer_cast<RewardedAd>(ad);
                item != nullptr) {
                item->onFailedToShow(message);
                return;
            }
        }
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
        displaying_ = false;
        if (auto iter = ads_.find(adId); iter != ads_.cend()) {
            auto&& ad = iter->second.second;
            if (auto item = std::dynamic_pointer_cast<InterstitialAd>(ad);
                item != nullptr) {
                item->onClosed();
                return;
            }
            if (auto item = std::dynamic_pointer_cast<RewardedAd>(ad);
                item != nullptr) {
                item->onClosed(rewarded);
                return;
            }
        }
    } else {
        // Mediation.
        onMediationAdClosed(adId, rewarded ? FullScreenAdResult::Completed
                                           : FullScreenAdResult::Canceled);
    }
}

void Self::onMediationAdFailedToShow(const std::string& adId,
                                     const std::string& message) {
    if (displayer_->isProcessing()) {
        displayer_->resolve(FullScreenAdResult::Failed);
        return;
    }
    assert(false);
}

void Self::onMediationAdClosed(const std::string& adId,
                               FullScreenAdResult result) {
    if (displayer_->isProcessing()) {
        displayer_->resolve(result);
        return;
    }
    assert(false);
}
} // namespace unity_ads
} // namespace ee
