//
//  FacebookRewardVideoAd.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/facebook_ads/private/FacebookRewardedAd.hpp"

#include <cassert>

#include <ee/ads/internal/AsyncHelper.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Logger.hpp>
#include <ee/core/Utils.hpp>

#include "ee/facebook_ads/FacebookAdsBridge.hpp"

namespace ee {
namespace facebook_ads {
using Self = RewardedAd;

Self::RewardedAd(
    IMessageBridge& bridge, const Logger& logger,
    const std::shared_ptr<ads::IAsyncHelper<IRewardedAdResult>>& displayer,
    Bridge* plugin, const std::string& adId)
    : bridge_(bridge)
    , logger_(logger)
    , displayer_(displayer)
    , plugin_(plugin)
    , adId_(adId)
    , messageHelper_("FacebookRewardedAd", adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
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
            onFailedToShow(message);
            return "";
        },
        messageHelper_.onFailedToShow());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClicked();
            return "";
        },
        messageHelper_.onClicked());
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClosed(core::toBool(message));
            return "";
        },
        messageHelper_.onClosed());
}

Self::~RewardedAd() {}

void Self::destroy() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());

    bridge_.deregisterHandler(messageHelper_.onLoaded());
    bridge_.deregisterHandler(messageHelper_.onFailedToLoad());
    bridge_.deregisterHandler(messageHelper_.onFailedToShow());
    bridge_.deregisterHandler(messageHelper_.onClicked());
    bridge_.deregisterHandler(messageHelper_.onClosed());

    auto succeeded = plugin_->destroyRewardedAd(adId_);
    assert(succeeded);
}

bool Self::createInternalAd() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    auto response = bridge_.call(messageHelper_.createInternalAd());
    return core::toBool(response);
}

bool Self::destroyInternalAd() {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId_.c_str());
    auto response = bridge_.call(messageHelper_.destroyInternalAd());
    return core::toBool(response);
}

bool Self::isLoaded() const {
    auto response = bridge_.call(messageHelper_.isLoaded());
    return core::toBool(response);
}

Task<bool> Self::load() {
    logger_.debug("%s: adId = %s loading = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(loader_->isProcessing()).c_str());
    auto result = co_await loader_->process(
        [this] { //
            bridge_.call(messageHelper_.load());
        },
        [this](bool result) {
            if (result) {
                // OK.
            } else {
                // Reinitialize internal ad if it was failed to load.
                destroyInternalAd();
                createInternalAd();
            }
        });
    co_return result;
}

Task<IRewardedAdResult> Self::show() {
    logger_.debug("%s: adId = %s displaying = %s", __PRETTY_FUNCTION__,
                  adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str());
    auto result = co_await displayer_->process(
        [this] { //
            bridge_.call(messageHelper_.show());
        },
        [this](IRewardedAdResult result) {
            if (result == IRewardedAdResult::Failed) {
                // Failed to show.
            } else {
                destroyInternalAd();
                createInternalAd();
            }
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
        logger_.error("%s: this ad is expected to be loading",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}

void Self::onFailedToShow(const std::string& message) {
    logger_.debug("%s: adId = %s displaying = %s  message = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str(),
                  message.c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(IRewardedAdResult::Failed);
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

void Self::onClosed(bool rewarded) {
    logger_.debug("%s: adId = %s displaying = %s rewarded = %s",
                  __PRETTY_FUNCTION__, adId_.c_str(),
                  core::toString(displayer_->isProcessing()).c_str(),
                  core::toString(rewarded).c_str());
    if (displayer_->isProcessing()) {
        displayer_->resolve(rewarded ? IRewardedAdResult::Completed
                                     : IRewardedAdResult::Canceled);
    } else {
        logger_.error("%s: this ad is expected to be displaying",
                      __PRETTY_FUNCTION__);
        assert(false);
    }
}
} // namespace facebook_ads
} // namespace ee
