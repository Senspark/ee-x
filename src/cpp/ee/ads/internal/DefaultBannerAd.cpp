//
//  DefaultBannerAd.cpp
//  Pods
//
//  Created by eps on 1/26/21.
//

#include "ee/ads/internal/DefaultBannerAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>

#include "ee/ad_mob/private/AdMobBridge.hpp"

namespace ee {
namespace ads {
using Self = DefaultBannerAd;

Self::DefaultBannerAd(const std::string& prefix, IMessageBridge& bridge,
                      const Logger& logger, const std::string& adId,
                      const Destroyer& destroyer,
                      const std::pair<int, int>& size)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer)
    , adId_(adId)
    , messageHelper_(prefix, adId)
    , helper_(bridge, messageHelper_, size) {
    logger_.debug("%s: prefix = %s adId = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());
    loader_ = std::make_unique<AsyncHelper<bool>>();

    bridge_.registerHandler(
        [this](const std::string& message) { //
            onLoaded();
        },
        messageHelper_.onLoaded());
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onFailedToLoad(message);
        },
        messageHelper_.onFailedToLoad());
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onClicked();
        },
        messageHelper_.onClicked());
}

Self::~DefaultBannerAd() = default;

void Self::destroy() {
    logger_.debug("%s: prefix = %s adId = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());

    bridge_.deregisterHandler(messageHelper_.onLoaded());
    bridge_.deregisterHandler(messageHelper_.onFailedToLoad());
    bridge_.deregisterHandler(messageHelper_.onClicked());

    destroyer_();
}

bool Self::isLoaded() const {
    return helper_.isLoaded();
}

Task<bool> Self::load() {
    logger_.debug("%s: prefix = %s adId = %s loading = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str(),
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
    logger_.debug("%s: prefix = %s adId = %s loading = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(true);
    } else {
        // Note: banner is auto-loading.
    }
    dispatchEvent([](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(const std::string& message) {
    logger_.debug("%s: prefix = %s adId = %s loading = %s message = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str(),
                  message.c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(false);
    } else {
        // Note: AdMob banner is auto-loading.
    }
}

void Self::onClicked() {
    logger_.debug("%s: prefix = %s adId = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}
} // namespace ads
} // namespace ee
