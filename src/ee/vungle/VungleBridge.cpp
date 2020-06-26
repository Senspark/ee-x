#include "ee/vungle/VungleBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/ads/internal/GuardedRewardedAd.hpp>
#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/vungle/private/VungleRewardedAd.hpp"

namespace ee {
namespace vungle {
using Self = Bridge;

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

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug(__PRETTY_FUNCTION__);
    PluginManager::addPlugin(Plugin::Vungle);

    auto&& mediation = ads::MediationManager::getInstance();
    rewardedAdDisplayer_ = mediation.getRewardedAdDisplayer();

    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onLoaded(json["ad_id"]);
            return "";
        },
        kOnLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToLoad(json["ad_id"], json["message"]);
            return "";
        },
        kOnFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToShow(json["ad_id"], json["message"]);
            return "";
        },
        kOnFailedToShow);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClicked(message);
            return "";
        },
        kOnClicked);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onClosed(json["ad_id"], json["rewarded"]);
            return "";
        },
        kOnClosed);
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug(__PRETTY_FUNCTION__);
    bridge_.deregisterHandler(kOnLoaded);
    bridge_.deregisterHandler(kOnFailedToLoad);
    bridge_.deregisterHandler(kOnFailedToShow);
    bridge_.deregisterHandler(kOnClosed);
    PluginManager::removePlugin(Plugin::Vungle);
}

void Self::initialize(const std::string& appId) {
    logger_.debug("%s: appId = %s", __PRETTY_FUNCTION__, appId.c_str());
    nlohmann::json json;
    json["appId"] = appId;
    bridge_.call(kInitialize, json.dump());
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
        iter->second.weak->onLoaded();
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onFailedToLoad(const std::string& adId, const std::string& message) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.weak->onFailedToLoad(message);
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onFailedToShow(const std::string& adId, const std::string& message) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.weak->onFailedToShow(message);
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onClicked(const std::string& adId) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.weak->onClicked();
    } else {
        // Mediation.
        assert(false);
    }
}

void Self::onClosed(const std::string& adId, bool rewarded) {
    logger_.debug(__PRETTY_FUNCTION__);
    auto iter = rewardedAds_.find(adId);
    if (iter != rewardedAds_.cend()) {
        iter->second.weak->onClosed(rewarded);
    } else {
        // Mediation.
        assert(false);
        onMediationAdClosed(adId, rewarded);
    }
}

void Self::onMediationAdClosed(const std::string& adId, bool rewarded) {
    logger_.debug("%s: %s", __PRETTY_FUNCTION__,
                  core::toString(rewarded).c_str());
    if (rewardedAdDisplayer_->isProcessing()) {
        rewardedAdDisplayer_->resolve(rewarded ? IRewardedAdResult::Completed
                                               : IRewardedAdResult::Canceled);
        return;
    }
    assert(false);
}
} // namespace vungle
} // namespace ee
