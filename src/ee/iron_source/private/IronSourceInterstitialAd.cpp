//
//  IronSourceInterstitialAd.cpp
//  ee_x
//
//  Created by Zinge on 10/10/17.
//
//

#include "ee/iron_source/private/IronSourceInterstitialAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/Logger.hpp>

#include "ee/iron_source/IronSourceBridge.hpp"

namespace ee {
namespace iron_source {
using Self = InterstitialAd;

Self::InterstitialAd(const Logger& logger,
                     const std::shared_ptr<ads::IAsyncHelper<bool>>& displayer,
                     Bridge* plugin, const std::string& adId)
    : logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug(__PRETTY_FUNCTION__);
}

Self::~InterstitialAd() {}

void Self::destroy() {
    logger_.debug("%s: begin", __PRETTY_FUNCTION__);
    plugin_->destroyInterstitialAd(adId_);
    logger_.debug("%s: end", __PRETTY_FUNCTION__);
}

bool Self::isLoaded() const {
    return plugin_->hasInterstitialAd();
}

Task<bool> Self::load() {
    auto result = co_await loader_->process(
        [this] { //
            plugin_->loadInterstitialAd();
        },
        [](bool result) {
            // OK.
        });
    co_return result;
}

Task<bool> Self::show() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto result = co_await displayer_->process(
        [this] { //
            plugin_->showInterstitialAd(adId_);
        },
        [](bool result) {
            // OK
        });
    co_return result;
}

void Self::onLoaded() {
    if (loader_->isProcessing()) {
        loader_->resolve(true);
    } else {
        assert(false);
    }
    dispatchEvent([&](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToLoad(const std::string& message) {
    if (loader_->isProcessing()) {
        loader_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onFailedToShow(const std::string& message) {
    if (displayer_->isProcessing()) {
        displayer_->resolve(false);
    } else {
        assert(false);
    }
}

void Self::onClicked() {
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}

void Self::onClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (displayer_->isProcessing()) {
        displayer_->resolve(true);
    } else {
        assert(false);
    }
}
} // namespace iron_source
} // namespace ee
