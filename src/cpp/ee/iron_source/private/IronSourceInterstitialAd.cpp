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
#include <ee/core/Utils.hpp>

#include "ee/iron_source/private/IronSourceBridge.hpp"

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
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();
}

Self::~InterstitialAd() = default;

void Self::destroy() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    plugin_->destroyInterstitialAd(adId_);
}

bool Self::isLoaded() const {
    return plugin_->hasInterstitialAd();
}

Task<bool> Self::load() {
    logger_.debug("%s: adId = %s loading = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
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
    logger_.debug("%s: adId = %s displaying = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
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
        // IronSource may dispatches multiple interstitialDidFailToLoadWithError
        // events due to mediation.
    }
}

void Self::onFailedToShow(const std::string& message) {
    logger_.debug("%s: adId = %s displaying = %s message = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str(),
                  message.c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(false);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
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

void Self::onClosed() {
    logger_.debug("%s: adId = %s displaying = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(true);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}
} // namespace iron_source
} // namespace ee
