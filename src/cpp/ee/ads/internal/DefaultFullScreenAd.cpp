//
//  DefaultFullScreenAd.cpp
//  Pods
//
//  Created by eps on 1/26/21.
//

#include "ee/ads/internal/DefaultFullScreenAd.hpp"

#include <cassert>

#include <ee/core/Delay.hpp>
#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/ads/internal/AsyncHelper.hpp"
#include "ee/ads/internal/Capper.hpp"

namespace ee {
namespace ads {
using Self = DefaultFullScreenAd;

Self::DefaultFullScreenAd(
    const std::string& prefix, IMessageBridge& bridge, ILogger& logger,
    const std::shared_ptr<IAsyncHelper<AdResult>>& displayer,
    const Destroyer& destroyer, const ResultParser& resultParser,
    const std::string& network, const std::string& adId)
    : prefix_(prefix)
    , bridge_(bridge)
    , logger_(logger)
    , displayer_(displayer)
    , destroyer_(destroyer)
    , resultParser_(resultParser)
    , network_(network)
    , adId_(adId)
    , messageHelper_(prefix, adId) {
    logger_.debug("%s: prefix = %s id = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());
    loadCapper_ = std::make_shared<Capper>(30);
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
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onFailedToShow(json["code"], json["message"]);
        },
        messageHelper_.onFailedToShow());
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onClicked();
        },
        messageHelper_.onClicked());
    bridge_.registerHandler(
        [this](const std::string& message) { //
            auto result = resultParser_(message);
            onClosed(result);
        },
        messageHelper_.onClosed());
}

Self::~DefaultFullScreenAd() = default;

void Self::destroy() {
    logger_.debug("%s: prefix = %s id = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());

    bridge_.deregisterHandler(messageHelper_.onLoaded());
    bridge_.deregisterHandler(messageHelper_.onFailedToLoad());
    bridge_.deregisterHandler(messageHelper_.onFailedToShow());
    bridge_.deregisterHandler(messageHelper_.onClicked());
    bridge_.deregisterHandler(messageHelper_.onClosed());

    destroyer_();
}

bool Self::isLoaded() const {
    auto response = bridge_.call(messageHelper_.isLoaded());
    return core::toBool(response);
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
            bridge_.call(messageHelper_.load());
        },
        [](bool result) {
            // OK.
        });
    co_return result;
}

Task<AdResult> Self::show() {
    logger_.debug("%s: prefix = %s id = %s displaying = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    auto result = co_await displayer_->process(
        [this] { //
            bridge_.call(messageHelper_.show());
        },
        [](AdResult result) {
            // OK.
        });
    co_return result;
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
        logger_.error("%s: this ad is expected to be loading",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}

void Self::onFailedToShow(int code, const std::string& message) {
    logger_.debug("%s: prefix = %s id = %s displaying = %s message = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str(),
                  message.c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(AdResult::Failed);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}

void Self::onClicked() {
    logger_.debug("%s: prefix = %s id = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}

void Self::onClosed(AdResult result) {
    logger_.debug("%s: prefix = %s id = %s displaying = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(result);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}
} // namespace ads
} // namespace ee
