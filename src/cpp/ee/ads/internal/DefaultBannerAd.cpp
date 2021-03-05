//
//  DefaultBannerAd.cpp
//  Pods
//
//  Created by eps on 1/26/21.
//

#include "ee/ads/internal/DefaultBannerAd.hpp"

#include <cassert>

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/ads/internal/AsyncHelper.hpp"
#include "ee/ads/internal/Capper.hpp"

namespace ee {
namespace ads {
using Self = DefaultBannerAd;

Self::DefaultBannerAd(const std::string& prefix, IMessageBridge& bridge,
                      ILogger& logger, const Destroyer& destroyer,
                      const std::string& network, const std::string& adId,
                      const std::pair<int, int>& size)
    : prefix_(prefix)
    , bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer)
    , network_(network)
    , adId_(adId)
    , messageHelper_(prefix, adId)
    , helper_(bridge, messageHelper_, size) {
    logger_.debug("%s: prefix = %s id = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());
    loadCapper_ = std::make_shared<Capper>(5);
    loader_ = std::make_unique<AsyncHelper<bool>>();

    bridge_.registerHandler(
        [this](const std::string& message) { //
            onLoaded();
        },
        messageHelper_.onLoaded());
    bridge_.registerHandler(
        [this](const std::string& message) { //
            auto json = nlohmann::json::parse(message);
            onFailedToLoad(json["code"], json["message"]);
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
    logger_.debug("%s: prefix = %s id = %s", __PRETTY_FUNCTION__,
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
    logger_.debug("%s: prefix = %s id = %s loading = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loadCapper_->isCapped()) {
        co_return false;
    }
    loadCapper_->cap();
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
    logger_.debug("%s: prefix = %s id = %s loading = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(true);
        dispatchEvent([this](auto&& observer) {
            if (observer.onLoadResult) {
                observer.onLoadResult({
                    .network = network_,
                    .result = true,
                });
            }
        });
    } else {
        // Note: banner is auto-loading.
    }
    dispatchEvent([](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(int code, const std::string& message) {
    logger_.debug("%s: prefix = %s id = %s loading = %s message = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str(),
                  message.c_str());
    if (loader_->isProcessing()) {
        loader_->resolve(false);
        dispatchEvent([this, code, message](auto&& observer) {
            if (observer.onLoadResult) {
                observer.onLoadResult({
                    .network = network_,
                    .result = false,
                    .errorCode = code,
                    .errorMessage = message,
                });
            }
        });
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
