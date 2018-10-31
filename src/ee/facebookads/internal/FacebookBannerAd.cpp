//
//  FacebookBannerAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include <cassert>

#include "ee/core/IMessageBridge.hpp"
#include "ee/facebookads/FacebookAdsBridge.hpp"
#include "ee/facebookads/internal/FacebookBannerAd.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace facebook {
using Self = BannerAd;

namespace {
auto k__onLoaded(const std::string& id) {
    return "FacebookBannerAd_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return "FacebookBannerAd_onFailedToLoad_" + id;
}

auto k__onClicked(const std::string& id) {
    return "FacebookBannerAd_onClicked_" + id;
}
} // namespace

Self::BannerAd(IMessageBridge& bridge, FacebookAds* plugin,
               const std::string& adId)
    : Super()
    , adId_(adId)
    , bridge_(bridge)
    , plugin_(plugin)
    , helper_("FacebookBannerAd", adId)
    , bridgeHelper_(bridge, helper_) {
    loading_ = false;

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
            onClicked();
            return "";
        },
        k__onClicked(adId_));
}

Self::~BannerAd() {
    bool succeeded = plugin_->destroyBannerAd(adId_);
    assert(succeeded);

    bridge_.deregisterHandler(k__onLoaded(adId_));
    bridge_.deregisterHandler(k__onFailedToLoad(adId_));
    bridge_.deregisterHandler(k__onClicked(adId_));
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
    // Facebook banner is auto-loading.
    // assert(loading_);
    loading_ = false;
    setLoadResult(true);
}

void Self::onFailedToLoad(const std::string& message) {
    // Facebook banner is auto-loading.
    // assert(loading_);
    loading_ = false;
    setLoadResult(false);
}

void Self::onClicked() {
    performClick();
}
} // namespace facebook
} // namespace ee
