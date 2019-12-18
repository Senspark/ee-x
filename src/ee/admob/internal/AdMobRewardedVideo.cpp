//
//  AdMobRewardedVideo.cpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#include <cassert>

#include "AdMobRewardedVideo.hpp"
#include "ee/admob/AdMobBridge.hpp"
#include "ee/admob/internal/AdMobRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/IMessageBridge.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"

namespace ee {
namespace admob {
using Self = RewardedVideo;

namespace {
auto k__createInternalAd(const std::string& id) {
    return "AdMobRewardVideoAd_createInternalAd_" + id;
}

auto k__destroyInternalAd(const std::string& id) {
    return "AdMobRewardVideoAd_destroyInternalAd_" + id;
}

auto k__isLoaded(const std::string& id) {
    return "AdMobRewardVideoAd_isLoaded_" + id;
}

auto k__load(const std::string& id) {
    return "AdMobRewardVideoAd_load_" + id;
}

auto k__show(const std::string& id) {
    return "AdMobRewardVideoAd_show_" + id;
}

auto k__onLoaded(const std::string& id) {
    return "AdMobRewardVideoAd_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "AdMobRewardVideoAd_onFailedToLoad_" + id;
}

auto k__onFailedToShow(const std::string& id) {
    return "AdMobRewardVideoAd_onFailedToShow_" + id;
}

auto k__onReward(const std::string& id) {
    return "AdMobRewardVideoAd_onReward_" + id;
}

auto k__onClosed(const std::string& id) {
    return "AdMobRewardVideoAd_onClosed_" + id;
}
} // namespace

Self::RewardedVideo(IMessageBridge& bridge, const Logger& logger, AdMob* plugin,
                    const std::string& adId)
    : Super(logger)
    , bridge_(bridge)
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    plugin_ = plugin;
    adId_ = adId;
    loading_ = false;
    errored_ = false;
    rewarded_ = false;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToShow();
            return "";
        },
        k__onFailedToShow(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onReward();
            return "";
        },
        k__onReward(adId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed(adId_));

    createInternalAd();
}

Self::~RewardedVideo() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    destroyInternalAd();
    
    bridge_.deregisterHandler(k__onLoaded(adId_));
    bridge_.deregisterHandler(k__onFailedToLoad(adId_));
    bridge_.deregisterHandler(k__onFailedToShow(adId_));
    bridge_.deregisterHandler(k__onReward(adId_));
    bridge_.deregisterHandler(k__onClosed(adId_));
    plugin_->destroyRewardedVideo(adId_);
}

bool Self::createInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    rewarded_ = false;
    auto response = bridge_.call(k__createInternalAd(adId_));
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__destroyInternalAd(adId_));
    return core::toBool(response);
}

bool Self::isLoaded() const {
    auto response = bridge_.call(k__isLoaded(adId_));
    return core::toBool(response);
}

void Self::load() {
    if (isLoaded()) {
        return;
    }
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loading_).c_str());
    if (loading_) {
        return;
    }
    loading_ = true;
    bridge_.call(k__load(adId_));
}

bool Self::show() {
    if (not isLoaded()) {
        return false;
    }
    logger_.debug("%s", __PRETTY_FUNCTION__);
    errored_ = false;
    bridge_.call(k__show(adId_));
    if (errored_) {
        return false;
    }
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startRewardedVideo([this](bool rewarded) {
        this->destroyInternalAd();
        this->createInternalAd();
        this->setResult(rewarded);
    });
    assert(successful);
    return true;
}

void Self::onLoaded() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loading_).c_str());
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    logger_.debug("%s: message = %s loading = %s", __PRETTY_FUNCTION__,
                  message.c_str(), core::toString(loading_).c_str());
    loading_ = false;
}

void Self::onFailedToShow() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (not errored_) {
        errored_ = true;
    }
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
} // namespace admob
} // namespace ee
