//
//  FacebookNativeAd.cpp
//  ee_x
//
//  Created by Zinge on 10/6/17.
//
//

#include "ee/facebook_ads/private/FacebookNativeAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/MakeAwaiter.hpp>
#include <ee/core/SwitchToUiThread.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>

#include "ee/facebook_ads/FacebookAdsBridge.hpp"

namespace ee {
namespace facebook_ads {
using Self = NativeAd;

Self::NativeAd(IMessageBridge& bridge, const Logger& logger, Bridge* plugin,
               const std::string& adId)
    : bridge_(bridge)
    , logger_(logger)
    , plugin_(plugin)
    , adId_(adId)
    , messageHelper_("FacebookNativeAd", adId)
    , helper_(bridge, messageHelper_, std::pair(0, 0)) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    attempted_ = false;
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

Self::~NativeAd() = default;

void Self::destroy() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());

    bridge_.deregisterHandler(messageHelper_.onLoaded());
    bridge_.deregisterHandler(messageHelper_.onFailedToLoad());
    bridge_.deregisterHandler(messageHelper_.onClosed());

    bool succeeded = plugin_->destroyNativeAd(adId_);
    assert(succeeded);
}

bool Self::createInternalAd() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    assert(Thread::isMainThread());
    auto response = bridge_.call(messageHelper_.createInternalAd());
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    assert(Thread::isMainThread());
    auto response = bridge_.call(messageHelper_.destroyInternalAd());
    return core::toBool(response);
}

bool Self::isLoaded() const {
    return helper_.isLoaded();
}

Task<bool> Self::load() {
    logger_.debug("%s: adId = %s loading = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    auto result = co_await loader_->process( //
        makeAwaiter([this]() -> Task<> {
            co_await SwitchToUiThread();
            if (attempted_) {
                destroyInternalAd();
                createInternalAd();
            }
            attempted_ = true;
            helper_.load();
        }),
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

std::pair<float, float> Self::getPosition() const {
    return helper_.getPosition();
}

void Self::setPosition(float x, float y) {
    helper_.setPosition(x, y);
}

std::pair<float, float> Self::getSize() const {
    return helper_.getSize();
}

void Self::setSize(float width, float height) {
    helper_.setSize(width, height);
}

bool Self::isVisible() const {
    return helper_.isVisible();
}

void Self::setVisible(bool visible) {
    helper_.setVisible(visible);
}

void Self::onLoaded() {
    logger_.debug("%s: adId = %s loading = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(true);
    } else {
        logger_.error("%s: this ad is expected to be loading",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
    dispatchEvent([](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(const std::string& message) {
    logger_.debug("%s: adId = %s loading = %s message = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str(),
                  message.c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(false);
    } else {
        logger_.error("%s: this ad is expected to be loading",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}

void Self::onClicked() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}
} // namespace facebook_ads
} // namespace ee
