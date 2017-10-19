//
//  FacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include <cassert>

#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"
#include "ee/facebook/FacebookAdsBridge.hpp"
#include "ee/facebook/internal/FacebookNativeAd.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = NativeAd;

namespace {
auto k__createInternalAd(const std::string& id) {
    return "FacebookNativeAd_createInternalAd_" + id;
}

auto k__destroyInternalAd(const std::string& id) {
    return "FacebookNativeAd_destroyInternalAd_" + id;
}

auto k__onLoaded(const std::string& id) {
    return "FacebookNativeAd_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "FacebookNativeAd_onFailedToLoad_" + id;
}
} // namespace

Self::NativeAd(FacebookAds* plugin, const std::string& adId)
    : Super()
    , adId_(adId)
    , plugin_(plugin)
    , helper_("FacebookNativeAd", adId)
    , bridgeHelper_(helper_) {
    attempted_ = false;
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
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onLoaded(adId_));
    bridge.deregisterHandler(k__onFailedToLoad(adId_));

    bool succeeded = plugin_->destroyNativeAd(adId_);
    assert(succeeded);
}

bool Self::createInternalAd() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createInternalAd(adId_));
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyInternalAd(adId_));
    return core::toBool(response);
}

bool Self::isLoaded() const {
    return bridgeHelper_.isLoaded();
}

void Self::load() {
    if (loading_) {
        return;
    }
    if (attempted_) {
        destroyInternalAd();
        createInternalAd();
    }
    attempted_ = true;
    bridgeHelper_.load();
}

std::pair<float, float> Self::getAnchor() const {
    return bridgeHelper_.getAnchor();
}

void Self::setAnchor(float x, float y) {
    bridgeHelper_.setAnchor(x, y);
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
    setLoadResult(true);
}

void Self::onFailedToLoad(const std::string& message) {
    assert(loading_);
    loading_ = false;
    setLoadResult(false);
}
} // namespace facebook
} // namespace ee
