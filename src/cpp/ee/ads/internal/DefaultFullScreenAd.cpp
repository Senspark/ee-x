//
//  DefaultFullScreenAd.cpp
//  Pods
//
//  Created by eps on 1/26/21.
//

#include "ee/ads/internal/DefaultFullScreenAd.hpp"

#include <ee/core/Delay.hpp>
#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>

#include "ee/ads/internal/AsyncHelper.hpp"

namespace ee {
namespace ads {
using Self = DefaultFullScreenAd;

Self::DefaultFullScreenAd(
    const std::string& prefix, IMessageBridge& bridge, ILogger& logger,
    const std::shared_ptr<IAsyncHelper<FullScreenAdResult>>& displayer,
    const Destroyer& destroyer, const ResultParser& resultParser,
    const std::string& adId)
    : prefix_(prefix)
    , bridge_(bridge)
    , logger_(logger)
    , displayer_(displayer)
    , destroyer_(destroyer)
    , resultParser_(resultParser)
    , adId_(adId)
    , messageHelper_(prefix, adId) {
    logger_.debug("%s: prefix = %s adId = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str());
    loadingCapped_ = false;
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
            onFailedToShow(message);
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
    logger_.debug("%s: prefix = %s adId = %s", __PRETTY_FUNCTION__,
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
    logger_.debug("%s: prefix = %s adId = %s loading = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    if (loadingCapped_) {
        co_return false;
    }
    loadingCapped_ = true;
    noAwait([this]() -> Task<> {
        co_await Delay(30.0f);
        loadingCapped_ = false;
    });
    auto result = co_await loader_->process(
        [this] { //
            bridge_.call(messageHelper_.load());
        },
        [](bool result) {
            // OK.
        });
    co_return result;
}

Task<FullScreenAdResult> Self::show() {
    logger_.debug("%s: prefix = %s adId = %s displaying = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    auto result = co_await displayer_->process(
        [this] { //
            bridge_.call(messageHelper_.show());
        },
        [](FullScreenAdResult result) {
            // OK.
        });
    co_return result;
}

void Self::onLoaded() {
    logger_.debug("%s: prefix = %s adId = %s loading = %s", __PRETTY_FUNCTION__,
                  prefix_.c_str(), adId_.c_str(),
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
    logger_.debug("%s: prefix = %s adId = %s loading = %s message = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
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

void Self::onFailedToShow(const std::string& message) {
    logger_.debug("%s: prefix = %s adId = %s displaying = %s message = %s",
                  __PRETTY_FUNCTION__, prefix_.c_str(), adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str(),
                  message.c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(FullScreenAdResult::Failed);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
        assert(false);
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

void Self::onClosed(FullScreenAdResult result) {
    logger_.debug("%s: prefix = %s adId = %s displaying = %s",
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
