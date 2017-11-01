//
//  AdMobInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#include <cassert>

#include "ee/admob/AdMobBridge.hpp"
#include "ee/admob/internal/AdMobInterstitialAd.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Logger.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"

namespace ee {
namespace admob {
using Self = InterstitialAd;

namespace {
auto k__createInternalAd(const std::string& id) {
    return "AdMobInterstitialAd_createInternalAd_" + id;
}

auto k__destroyInternalAd(const std::string& id) {
    return "AdMobInterstitialAd_destroyInternalAd_" + id;
}

auto k__isLoaded(const std::string& id) {
    return "AdMobInterstitialAd_isLoaded_" + id;
}

auto k__load(const std::string& id) {
    return "AdMobInterstitialAd_load_" + id;
}

auto k__show(const std::string& id) {
    return "AdMobInterstitialAd_show_" + id;
}

auto k__onLoaded(const std::string& id) {
    return "AdMobInterstitialAd_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "AdMobInterstitialAd_onFailedToLoad_" + id;
}

auto k__onFailedToShow(const std::string& id) {
    return "AdMobInterstitialAd_onFailedToShow_" + id;
}

auto k__onClosed(const std::string& id) {
    return "AdMobInterstitialAd_onClosed_" + id;
}
} // namespace

Self::InterstitialAd(AdMob* plugin, const std::string& adId) {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    loading_ = false;
    errored_ = false;
    plugin_ = plugin;
    adId_ = adId;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded(adId_));
    bridge.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad(adId_));
    bridge.registerHandler(
        [this](const std::string& message) {
            onFailedToShow();
            return "";
        },
        k__onFailedToShow(adId_));
    bridge.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed(adId_));

    createInternalAd();
}

Self::~InterstitialAd() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    destroyInternalAd();

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onLoaded(adId_));
    bridge.deregisterHandler(k__onFailedToLoad(adId_));
    bridge.deregisterHandler(k__onFailedToShow(adId_));
    bridge.deregisterHandler(k__onClosed(adId_));
    plugin_->destroyInterstitialAd(adId_);
}

bool Self::createInternalAd() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createInternalAd(adId_));
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyInternalAd(adId_));
    return core::toBool(response);
}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__isLoaded(adId_));
    return core::toBool(response);
}

void Self::load() {
    if (isLoaded()) {
        return;
    }
    if (loading_) {
        return;
    }
    loading_ = true;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__load(adId_));
}

bool Self::show() {
    if (not isLoaded()) {
        return false;
    }
    errored_ = false;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__show(adId_));
    if (errored_) {
        return false;
    }
    auto&& mediation = ads::MediationManager::getInstance();
    auto successful = mediation.startInterstitialAd(this);
    assert(successful);
    return true;
}

void Self::onLoaded() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    Logger::getSystemLogger().debug("%s: message = %s", __PRETTY_FUNCTION__,
                                    message.c_str());
    loading_ = false;
}

void Self::onFailedToShow() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    if (not errored_) {
        errored_ = true;
    }
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
} // namespace admob
} // namespace ee
