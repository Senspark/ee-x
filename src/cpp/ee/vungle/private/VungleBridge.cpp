#include "ee/vungle/private/VungleBridge.hpp"

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

#include "ee/vungle/private/VungleRewardedAd.hpp"

namespace ee {
namespace core {
template <>
std::shared_ptr<IVungle>
PluginManager::createPluginImpl(IMessageBridge& bridge) {
    addPlugin(Plugin::Vungle);
    return std::make_shared<vungle::Bridge>(bridge);
}
} // namespace core

namespace vungle {
namespace {
// clang-format off
const std::string kPrefix    = "VungleBridge";

const auto kInitialize     = kPrefix + "Initialize";

const auto kHasRewardedAd  = kPrefix + "HasRewardedAd";
const auto kLoadRewardedAd = kPrefix + "LoadRewardedAd";
const auto kShowRewardedAd = kPrefix + "ShowRewardedAd";

const auto kOnLoaded       = kPrefix + "OnLoaded";
const auto kOnFailedToLoad = kPrefix + "OnFailedToLoad";
const auto kOnFailedToShow = kPrefix + "OnFailedToShow";
const auto kOnClicked      = kPrefix + "OnClicked";
const auto kOnClosed       = kPrefix + "OnClosed";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge)
    : bridge_(bridge)
    , logger_(Logger::getSystemLogger()) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    displayer_ = mediation.getAdDisplayer();

    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onLoaded(json["ad_id"]);
        },
        kOnLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToLoad(json["ad_id"], json["message"]);
        },
        kOnFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToShow(json["ad_id"], json["message"]);
        },
        kOnFailedToShow);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onClicked(message);
        },
        kOnClicked);
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
    bridge_.deregisterHandler(kOnFailedToLoad);
    bridge_.deregisterHandler(kOnFailedToShow);
    bridge_.deregisterHandler(kOnClicked);
    bridge_.deregisterHandler(kOnClosed);
    PluginManager::removePlugin(Plugin::Vungle);
}

Task<bool> Self::initialize(const std::string& appId) {
    logger_.debug("%s: appId = %s", __PRETTY_FUNCTION__, appId.c_str());
    nlohmann::json json;
    json["appId"] = appId;
    auto response = co_await bridge_.callAsync(kInitialize, json.dump());
    co_await SwitchToLibraryThread();
    co_return core::toBool(response);
}

std::shared_ptr<IFullScreenAd> Self::createRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        return iter->second.ad;
    }
    auto raw = std::make_shared<RewardedAd>(logger_, displayer_, this, adId);
    auto ad = std::make_shared<ads::GuardedFullScreenAd>(raw);
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
    auto result = bridge_.call(kHasRewardedAd, adId);
    return core::toBool(result);
}

void Self::loadRewardedAd(const std::string& adId) const {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    bridge_.call(kLoadRewardedAd, adId);
}

void Self::showRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    bridge_.call(kShowRewardedAd, adId);
}

void Self::onLoaded(const std::string& adId) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.raw->onLoaded();
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onFailedToLoad(const std::string& adId, const std::string& message) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.raw->onFailedToLoad(message);
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onFailedToShow(const std::string& adId, const std::string& message) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.raw->onFailedToShow(message);
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onClicked(const std::string& adId) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.raw->onClicked();
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onClosed(const std::string& adId, bool rewarded) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.raw->onClosed(rewarded);
    } else {
        // Mediation.
        onMediationAdClosed(adId, rewarded);
    }
}

void Self::onMediationAdClosed(const std::string& adId, bool rewarded) {
    logger_.debug("%s: %s", __PRETTY_FUNCTION__,
                  core::toString(rewarded).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(rewarded ? FullScreenAdResult::Completed
                                     : FullScreenAdResult::Canceled);
        return;
    }
    assert(false);
}
} // namespace vungle
} // namespace ee
