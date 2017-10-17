//
//  AdMobNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#include "ee/admob/internal/AdMobNativeAd.hpp"
#include "ee/admob/AdMobBridge.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace admob {
using Self = NativeAd;

namespace {
auto k__onLoaded(const std::string& id) {
    return "AdMobNativeAd_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "AdMobNativeAd_onFailedToLoad_" + id;
}
} // namespace

Self::NativeAd(AdMob* plugin, const std::string& adId)
    : Super()
    , adId_(adId)
    , plugin_(plugin)
    , helper_("AdMobNativeAd", adId)
    , bridgeHelper_(helper_) {
    loading_ = false;

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
}

Self::~NativeAd() {
    bool succeeded = plugin_->destroyNativeAd(adId_);
    assert(succeeded);

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onLoaded(adId_));
    bridge.deregisterHandler(k__onFailedToLoad(adId_));
}

bool Self::isLoaded() const {
    return bridgeHelper_.isLoaded();
}

void Self::load() {
    if (loading_) {
        return;
    }
    loading_ = true;
    bridgeHelper_.load();
}

std::pair<int, int> Self::getPosition() const {
    return bridgeHelper_.getPosition();
}

void Self::setPosition(int x, int y) {
    bridgeHelper_.setPosition(x, y);
}

std::pair<int, int> Self::getSize() const {
    return bridgeHelper_.getSize();
}

void Self::setSize(int width, int height) {
    bridgeHelper_.setSize(width, height);
}

void Self::setVisible(bool visible) {
    bridgeHelper_.setVisible(visible);
}

void Self::onLoaded() {
    assert(loading_);
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    assert(loading_);
    loading_ = false;
}
} // namespace admob
} // namespace ee