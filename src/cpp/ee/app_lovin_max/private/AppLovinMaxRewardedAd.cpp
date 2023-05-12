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
                 Bridge* plugin, const std::string& adId)
    : logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , adId_(adId) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
}

Self::~RewardedAd() = default;

void Self::destroy() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    plugin_->destroyRewardedAd(adId_);
}

bool Self::isLoaded() const {
    return plugin_->hasRewardedAd();
}

Task<bool> Self::load() {
    // Should return whether this ad is loaded.
    noAwait([this]() -> Task<> { //
        co_await plugin_->loadRewardedAd();
    });
    // Should return whether this ad is loaded.
    co_return isLoaded();
}

Task<AdResult> Self::show() {
    logger_.debug("%s: adId = %s displaying = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    auto result = co_await displayer_->process(
        [this] { //
            plugin_->showRewardedAd(adId_);
        },
        [](AdResult result) {
            // OK.
        });
    co_return result;
}

void Self::onLoaded() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    dispatchEvent([](auto&& observer) {
        if (observer.onLoaded) {
            observer.onLoaded();
        }
    });
}

void Self::onFailedToShow(int code, const std::string& message) {
    logger_.debug("%s: adId = %s displaying = %s message = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str(),
                  message.c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(AdResult::Failed);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
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

void Self::onClosed(bool rewarded) {
    logger_.debug("%s: adId = %s displaying = %s rewarded = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str(),
                  core::toString(rewarded).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(rewarded ? AdResult::Completed
                                     : AdResult::Canceled);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
    }
}
} // namespace app_lovin_max
} // namespace ee
