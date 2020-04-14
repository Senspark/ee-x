//
//  FacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "ee/facebook_ads/private/FacebookNativeAd.hpp"

#include <cassert>

#include <ee/nlohmann/json.hpp>

#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/IMessageBridge.hpp>

#include "ee/facebook_ads/FacebookAdsBridge.hpp"

namespace ee {
namespace facebook_ads {
using Self = NativeAd;

namespace {
const std::string k__tag = "FacebookNativeAd";

auto k__createInternalAd(const std::string& id) {
    return k__tag + "_createInternalAd_" + id;
}

auto k__destroyInternalAd(const std::string& id) {
    return k__tag + "_destroyInternalAd_" + id;
}

auto k__onLoaded(const std::string& id) {
    return k__tag + "_onLoaded_" + id;
}

auto k__onFailedToLoad(const std::string& id) {
    return k__tag + "_onFailedToLoad_" + id;
}

auto k__onClicked(const std::string& id) {
    return k__tag + "_onClicked_" + id;
}
} // namespace

Self::NativeAd(IMessageBridge& bridge, const Logger& logger, Bridge* plugin,
               const std::string& adId)
    : Super()
    , adId_(adId)
    , bridge_(bridge)
    , logger_(logger)
    , plugin_(plugin)
    , helper_(bridge, "FacebookNativeAd", adId) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    attempted_ = false;
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

Self::~NativeAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onLoaded(adId_));
    bridge_.deregisterHandler(k__onFailedToLoad(adId_));
    bridge_.deregisterHandler(k__onClicked(adId_));

    bool succeeded = plugin_->destroyNativeAd(adId_);
    assert(succeeded);
}

bool Self::createInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__createInternalAd(adId_));
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = bridge_.call(k__destroyInternalAd(adId_));
    return core::toBool(response);
}

bool Self::isLoaded() const {
    return helper_.isLoaded();
}

void Self::load() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loading_).c_str());
    if (loading_) {
        return;
    }
    if (attempted_) {
        destroyInternalAd();
        createInternalAd();
    }
    loading_ = true;
    attempted_ = true;
    helper_.load();
}

std::pair<float, float> Self::getAnchor() const {
    return helper_.getAnchor();
}

void Self::setAnchor(float x, float y) {
    helper_.setAnchor(x, y);
}

std::pair<int, int> Self::getPosition() const {
    return helper_.getPosition();
}

void Self::setPosition(int x, int y) {
    helper_.setPosition(x, y);
}

std::pair<int, int> Self::getSize() const {
    return helper_.getSize();
}

void Self::setSize(int width, int height) {
    helper_.setSize(width, height);
}

bool Self::isVisible() const {
    return helper_.isVisible();
}

void Self::setVisible(bool visible) {
    helper_.setVisible(visible);
}

void Self::onLoaded() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    assert(loading_);
    loading_ = false;
    dispatchEvent([](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(const std::string& message) {
    logger_.debug("%s: message = %s", __PRETTY_FUNCTION__, message.c_str());
    assert(loading_);
    loading_ = false;
    dispatchEvent([](auto&& observer) {
        if (observer.onFailedToLoad) {
            observer.onFailedToLoad();
        }
    });
}

void Self::onClicked() {
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}
} // namespace facebook_ads
} // namespace ee
