//
//  AdMobInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/12/17.
//
//

#include "ee/admob/internal/AdMobInterstitialAd.hpp"
#include "ee/admob/AdMobBridge.hpp"
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
    destroyInternalAd();

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onLoaded(adId_));
    bridge.deregisterHandler(k__onFailedToLoad(adId_));
    bridge.deregisterHandler(k__onFailedToShow(adId_));
    bridge.deregisterHandler(k__onClosed(adId_));
    plugin_->destroyInterstitialAd(adId_);
}

bool Self::createInternalAd() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createInternalAd(adId_));
    return response == "true";
}

bool Self::destroyInternalAd() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyInternalAd(adId_));
    return response == "true";
}

bool Self::isLoaded() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__isLoaded(adId_));
    return response == "true";
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
    return not errored_;
}

void Self::onLoaded() {
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    loading_ = false;
}

void Self::onFailedToShow() {
    if (not errored_) {
        errored_ = true;
    }
}

void Self::onClosed() {
    destroyInternalAd();
    createInternalAd();
    setDone();
}
} // namespace admob
} // namespace ee
