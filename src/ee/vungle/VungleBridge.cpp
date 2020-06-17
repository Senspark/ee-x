#include "ee/vungle/VungleBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/ads/internal/GuardedRewardedAd.hpp>
#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/PluginManager.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/vungle/private/VungleRewardedAd.hpp"

namespace ee {
namespace vungle {
using Self = Bridge;

namespace {
// clang-format off
const std::string kPrefix    = "Vungle";

const auto k__initialize     = kPrefix + "_initialize";

const auto k__hasRewardedAd  = kPrefix + "_hasRewardedAd";
const auto k__loadRewardedAd = kPrefix + "_loadRewardedAd";
const auto k__showRewardedAd = kPrefix + "_showRewardedAd";

const auto k__onLoaded       = kPrefix + "_onLoaded";
const auto k__onFailedToLoad = kPrefix + "_onFailedToLoad";
const auto k__onFailedToShow = kPrefix + "_onFailedToShow";
const auto k__onClosed       = kPrefix + "_onClosed";
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
        k__onLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToLoad(json["ad_id"], json["message"]);
            return "";
        },
        k__onFailedToLoad);
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
    bridge_.deregisterHandler(k__onFailedToLoad);
    bridge_.deregisterHandler(k__onFailedToShow);
    bridge_.deregisterHandler(k__onClosed);
    PluginManager::removePlugin(Plugin::Vungle);
}

void Self::initialize(const std::string& gameId) {
    logger_.debug("%s: gameId = %s", __PRETTY_FUNCTION__, gameId.c_str());
    runOnMainThread([this, gameId] {
        nlohmann::json json;
        json["gameId"] = gameId;
        bridge_.call(k__initialize, json.dump());
    });
}

void Self::initialize(const std::string& gameId, const std::string& adId) {
    initialize(gameId);
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
    auto result = bridge_.call(k__hasRewardedAd, adId);
    return core::toBool(result);
}

void Self::loadRewardedAd(const std::string& adId) const {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    runOnMainThread([this, adId] { //
        bridge_.call(k__loadRewardedAd, adId);
    });
}

void Self::showRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    runOnMainThread([this, adId] { //
        bridge_.call(k__showRewardedAd, adId);
    });
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
