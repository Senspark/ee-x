//
//  FacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/facebookads/private/FacebookInterstitialAd.hpp"

#include <cassert>

#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/IMessageBridge.hpp>

#include "ee/facebookads/FacebookAdsBridge.hpp"

namespace ee {
namespace facebook {
using Self = InterstitialAd;

namespace {
auto k__createInternalAd(const std::string& id) {
    return "FacebookInterstitialAd_createInternalAd_" + id;
}

auto k__destroyInternalAd(const std::string& id) {
    return "FacebookInterstitialAd_destroyInternalAd_" + id;
}

auto k__isLoaded(const std::string& id) {
    return "FacebookInterstitialAd_isLoaded_" + id;
}

auto k__load(const std::string& id) {
    return "FacebookInterstitialAd_load_" + id;
}

auto k__show(const std::string& id) {
    return "FacebookInterstitialAd_show_" + id;
}

auto k__onLoaded(const std::string& id) {
    return "FacebookInterstitialAd_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "FacebookInterstitialAd_onFailedToLoad_" + id;
}

auto k__onClosed(const std::string& id) {
    return "FacebookInterstitialAd_onClosed_" + id;
}

auto k__onClicked(const std::string& id) {
    return "FacebookInterstitialAd_onClicked_" + id;
}
} // namespace

Self::InterstitialAd(IMessageBridge& bridge, const Logger& logger,
                     FacebookAds* plugin, const std::string& placementId)
    : bridge_(bridge)
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    loading_ = false;
    plugin_ = plugin;
    placementId_ = placementId;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded(placementId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad(placementId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed(placementId_));
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClicked();
            return "";
        },
        k__onClicked(placementId_));
}

Self::~InterstitialAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onLoaded(placementId_));
    bridge_.deregisterHandler(k__onFailedToLoad(placementId_));
    bridge_.deregisterHandler(k__onClosed(placementId_));
    bridge_.deregisterHandler(k__onClicked(placementId_));
    plugin_->destroyInterstitialAd(placementId_);
}

bool Self::createInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__createInternalAd(placementId_));
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__destroyInternalAd(placementId_));
    return core::toBool(response);
}

bool Self::isLoaded() const {
    auto response = bridge_.call(k__isLoaded(placementId_));
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
    bridge_.call(k__load(placementId_));
}

bool Self::show() {
    if (not isLoaded()) {
        return false;
    }
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.call(k__show(placementId_));
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startInterstitialAd([this]() {
        this->destroyInternalAd();
        this->createInternalAd();
        this->setDone();
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

void Self::onClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();

    auto successful = mediation.finishInterstitialAd();
    assert(successful);
}

void Self::onClicked() {
    performClick();
}
} // namespace facebook
} // namespace ee
