//
//  AdMobNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/13/17.
//
//

#include "ee/admob/private/AdMobNativeAd.hpp"

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/IMessageBridge.hpp>

#include "ee/admob/AdMobBridge.hpp"

namespace ee {
namespace admob {
using Self = NativeAd;

Self::NativeAd(IMessageBridge& bridge, const Logger& logger, AdMob* plugin,
               const std::string& adId)
    : bridge_(bridge)
    , logger_(logger)
    , plugin_(plugin)
    , adId_(adId)
    , messageHelper_("AdMobNativeAd", adId)
    , helper_(bridge, messageHelper_) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();

    bridge_.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        messageHelper_.onLoaded());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        messageHelper_.onFailedToLoad());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClicked();
            return "";
        },
        messageHelper_.onClicked());
}

Self::~NativeAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.deregisterHandler(messageHelper_.onLoaded());
    bridge_.deregisterHandler(messageHelper_.onFailedToLoad());
    bridge_.deregisterHandler(messageHelper_.onClicked());

    bool succeeded = plugin_->destroyNativeAd(adId_);
    assert(succeeded);
}

bool Self::isLoaded() const {
    return helper_.isLoaded();
}

Task<bool> Self::load() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loader_->isProcessing()).c_str());
    auto result = co_await loader_->process(
        [this] { //
            helper_.load();
        },
        [](bool result) {
            // OK.
        });
    co_return result;
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
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(true);
    } else {
        // Auto refresh customized in server.
    }
    dispatchEvent([](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(const std::string& message) {
    logger_.debug("%s: message = %s loading = %s", __PRETTY_FUNCTION__,
                  message.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onClicked() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}
} // namespace admob
} // namespace ee
