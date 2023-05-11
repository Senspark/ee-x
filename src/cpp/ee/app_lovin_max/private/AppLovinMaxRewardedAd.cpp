#include "ee/app_lovin_max/private/AppLovinMaxRewardedAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/ILogger.hpp>
#include <ee/core/Utils.hpp>

#include "ee/app_lovin_max/private/AppLovinMaxBridge.hpp"

namespace ee {
namespace app_lovin_max {
using Self = RewardedAd;

Self::RewardedAd(ILogger& logger,
                 const std::shared_ptr<ads::IAsyncHelper<AdResult>>& displayer,
                 Bridge* plugin, const std::string& network)
    : logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , network_(network) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    loader_ = std::make_unique<ads::AsyncHelper<bool>>();
}

Self::~RewardedAd() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    plugin_->destroyRewardedAd();
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd();
}

Task<bool> Self::load() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
                  core::toString(loader_->isProcessing()).c_str());
    auto result = co_await loader_->process(
        [this] { //
            plugin_->loadRewardedAd();
        },
        [](bool result) {
            // OK.
        });
    co_return result;
}

Task<AdResult> Self::show() {
    logger_.debug("%s: displaying = %s", __PRETTY_FUNCTION__,
                  core::toString(displayer_->isProcessing()).c_str());
    auto result = co_await displayer_->process(
        [this] { //
            plugin_->showRewardedAd();
        },
        [](AdResult result) {
            // OK.
        });
    co_return result;
}

void Self::onLoaded() {
    logger_.debug("%s: loading = %s", __PRETTY_FUNCTION__,
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
    logger_.debug("%s: loading = %s message = %s", __PRETTY_FUNCTION__,
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

void Self::onClicked() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    dispatchEvent([](auto&& observer) {
        if (observer.onClicked) {
            observer.onClicked();
        }
    });
}

void Self::onClosed(bool rewarded) {
    logger_.debug("%s: displaying = %s rewarded = %s", __PRETTY_FUNCTION__,
                  core::toString(displayer_->isProcessing()).c_str(),
                  core::toString(rewarded).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(rewarded ? AdResult::Completed
                                     : AdResult::Canceled);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}
} // namespace app_lovin
} // namespace ee
