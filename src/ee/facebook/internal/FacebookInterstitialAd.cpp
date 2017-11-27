//
//  FacebookInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include <cassert>

#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/facebook/FacebookAdsBridge.hpp"
#include "ee/facebook/internal/FacebookInterstitialAd.hpp"

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
} // namespace

Self::InterstitialAd(FacebookAds* plugin, const std::string& placementId) {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    loading_ = false;
    plugin_ = plugin;
    placementId_ = placementId;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded(placementId_));
    bridge.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad(placementId_));
    bridge.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed(placementId_));

    createInternalAd();
}

Self::~InterstitialAd() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    destroyInternalAd();

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onLoaded(placementId_));
    bridge.deregisterHandler(k__onFailedToLoad(placementId_));
    bridge.deregisterHandler(k__onClosed(placementId_));
    plugin_->destroyInterstitialAd(placementId_);
}

bool Self::createInternalAd() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createInternalAd(placementId_));
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyInternalAd(placementId_));
    return core::toBool(response);
}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__isLoaded(placementId_));
    return core::toBool(response);
}

void Self::load() {
    if (isLoaded()) {
        return;
    }
    Logger::getSystemLogger().debug("%s: loading = %s", __PRETTY_FUNCTION__,
                                    core::toString(loading_).c_str());
    if (loading_) {
        return;
    }
    loading_ = true;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__load(placementId_));
}

bool Self::show() {
    if (not isLoaded()) {
        return false;
    }
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__show(placementId_));
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startInterstitialAd(this);
    assert(successful);
    return true;
}

void Self::onLoaded() {
    Logger::getSystemLogger().debug("%s: loading = %s", __PRETTY_FUNCTION__,
                                    core::toString(loading_).c_str());
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    Logger::getSystemLogger().debug("%s: message = %s loading = %s",
                                    __PRETTY_FUNCTION__, message.c_str(),
                                    core::toString(loading_).c_str());
    loading_ = false;
}

void Self::onClosed() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    destroyInternalAd();
    createInternalAd();
    setDone();
    auto successful = mediation.finishInterstitialAd(this);
    assert(successful);
}
} // namespace facebook
} // namespace ee
