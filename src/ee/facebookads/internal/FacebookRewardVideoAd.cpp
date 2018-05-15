//
//  FacebookRewardVideoAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include <cassert>

#include "ee/core/Logger.hpp"
#include "ee/core/IMessageBridge.hpp"
#include "ee/core/Utils.hpp"
#include "ee/facebookads/FacebookAdsBridge.hpp"
#include "ee/facebookads/internal/FacebookRewardVideoAd.hpp"
#include "ee/ads/internal/MediationManager.hpp"

namespace ee {
namespace facebook {
using Self = RewardedVideo;

namespace {
auto k__createInternalVideo(const std::string& id) {
    return "FacebookAds_createInternalVideo_" + id;
}

auto k__destroyInternalVideo(const std::string& id) {
    return "FacebookAds_destroyInternalVideo_" + id;
}

auto k__hasRewardedVideo(const std::string& id) {
    return "FacebookAds_hasRewardedVideo_" + id;
}

auto k__loadRewardedVideo(const std::string& id) {
    return "FacebookAds_loadRewardedVideo_" + id;
}

auto k__showRewardedVideo(const std::string& id) {
    return "FacebookAds_showRewardedVideo_" + id;
}

auto k__onRewarded(const std::string& id) {
    return "FacebookAds_Video_onRewarded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "FacebookAds_Video_onFailedToLoad_" + id;
}
auto k__onLoaded(const std::string& id) {
    return "FacebookAds_Video_onLoaded_" + id;
}
auto k__onOpened(const std::string& id) {
    return "FacebookAds_Video_onOpened_" + id;
}
auto k__onClosed(const std::string& id) {
    return "FacebookAds_Video_onClosed_" + id;
}
} // namespace

Self::RewardedVideo(IMessageBridge& bridge, Logger& logger,
                    const std::string& adId)
    : Super(logger)
    , bridge_(bridge)
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    adId_ = adId;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onReward();
            return "";
        },
        k__onRewarded(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onOpened();
            return "";
        },
        k__onOpened(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed(adId_));
}

Self::~RewardedVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onRewarded(adId_));
    bridge_.deregisterHandler(k__onFailedToLoad(adId_));
    bridge_.deregisterHandler(k__onLoaded(adId_));
    bridge_.deregisterHandler(k__onOpened(adId_));
    bridge_.deregisterHandler(k__onClosed(adId_));
}

bool Self::isLoaded() const {
    auto response = bridge_.call(k__hasRewardedVideo(adId_));
    return core::toBool(response);
}

void Self::load() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.call(k__loadRewardedVideo(adId_));
}

bool Self::show() {
    if (not isLoaded()) {
        return false;
    }

    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__showRewardedVideo(adId_));
    if (not core::toBool(response)) {
        return false;
    }

    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo([this](bool rewarded) { //
        this->setResult(rewarded);

        this->destroyInternalVideo();
        this->createInternalVideo();
    });
    assert(successful);
    return true;
}

void Self::createInternalVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__createInternalVideo(adId_));
}

void Self::destroyInternalVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__destroyInternalVideo(adId_));
}
#pragma mark - on
void Self::onLoaded() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

void Self::onFailedToLoad(const std::string& message) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

void Self::onOpened() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    rewarded_ = false;
}

void Self::onReward() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    rewarded_ = true;
}

void Self::onClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.finishRewardedVideo(rewarded_);
    assert(successful);
}
} // namespace facebook
} // namespace ee
