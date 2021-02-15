#include "ee/iron_source/private/IronSourceBridge.hpp"

#include <ee/ads/internal/DefaultBannerAd.hpp>
#include <ee/ads/internal/GuardedBannerAd.hpp>
#include <ee/ads/internal/GuardedFullScreenAd.hpp>
#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/iron_source/private/IronSourceInterstitialAd.hpp"
#include "ee/iron_source/private/IronSourceRewardedAd.hpp"

namespace ee {
namespace iron_source {
namespace {
const std::string kPrefix = "IronSourceBridge";
const auto kInitialize = kPrefix + "Initialize";
const auto kGetBannerAdSize = kPrefix + "GetBannerAdSize";
const auto kCreateBannerAd = kPrefix + "CreateBannerAd";
const auto kDestroyAd = kPrefix + "DestroyAd";
const auto kHasInterstitialAd = kPrefix + "HasInterstitialAd";
const auto kLoadInterstitialAd = kPrefix + "LoadInterstitialAd";
const auto kShowInterstitialAd = kPrefix + "ShowInterstitialAd";
const auto kHasRewardedAd = kPrefix + "HasRewardedAd";
const auto kShowRewardedAd = kPrefix + "ShowRewardedAd";
const auto kOnInterstitialAdLoaded = kPrefix + "OnInterstitialAdLoaded";
const auto kOnInterstitialAdFailedToLoad =
    kPrefix + "OnInterstitialAdFailedToLoad";
const auto kOnInterstitialAdFailedToShow =
    kPrefix + "OnInterstitialAdFailedToShow";
const auto kOnInterstitialAdClicked = kPrefix + "OnInterstitialAdClicked";
const auto kOnInterstitialAdClosed = kPrefix + "OnInterstitialAdClosed";
const auto kOnRewardedAdLoaded = kPrefix + "OnRewardedAdLoaded";
const auto kOnRewardedAdFailedToShow = kPrefix + "OnRewardedAdFailedToShow";
const auto kOnRewardedAdClicked = kPrefix + "OnRewardedAdClicked";
const auto kOnRewardedAdClosed = kPrefix + "OnRewardedAdClosed";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    displayer_ = mediation.getAdDisplayer();
    interstitialAd_ = nullptr;
    rewardedAd_ = nullptr;

    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdLoaded();
        },
        kOnInterstitialAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdFailedToLoad(message);
        },
        kOnInterstitialAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdFailedToShow(message);
        },
        kOnInterstitialAdFailedToShow);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdClicked();
        },
        kOnInterstitialAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdClosed();
        },
        kOnInterstitialAdClosed);

    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdLoaded();
        },
        kOnRewardedAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdFailedToShow(message);
        },
        kOnRewardedAdFailedToShow);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdClicked();
        },
        kOnRewardedAdClicked);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdClosed(core::toBool(message));
        },
        kOnRewardedAdClosed);
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.deregisterHandler(kOnInterstitialAdLoaded);
    bridge_.deregisterHandler(kOnInterstitialAdFailedToLoad);
    bridge_.deregisterHandler(kOnRewardedAdFailedToShow);
    bridge_.deregisterHandler(kOnInterstitialAdClicked);
    bridge_.deregisterHandler(kOnInterstitialAdClosed);

    bridge_.deregisterHandler(kOnRewardedAdLoaded);
    bridge_.deregisterHandler(kOnRewardedAdFailedToShow);
    bridge_.deregisterHandler(kOnRewardedAdClicked);
    bridge_.deregisterHandler(kOnRewardedAdClosed);

    destroyer_();
}

Task<bool> Self::initialize(const std::string& appKey) {
    logger_.debug("%s: appKey = %s", __PRETTY_FUNCTION__, appKey.c_str());
    auto response = co_await bridge_.callAsync(kInitialize, appKey);
    co_return core::toBool(response);
}

std::pair<int, int> Self::getBannerAdSize(BannerAdSize adSize) {
    auto response = bridge_.call(kGetBannerAdSize,
                                 std::to_string(static_cast<int>(adSize)));
    auto json = nlohmann::json::parse(response);
    int width = json["width"];
    int height = json["height"];
    return std::pair(width, height);
}

std::shared_ptr<IBannerAd> Self::createBannerAd(const std::string& adId,
                                                BannerAdSize adSize) {
    logger_.debug("%s: id = %s size = %d", __PRETTY_FUNCTION__, adId.c_str(),
                  static_cast<int>(adSize));
    if (bannerAd_) {
        return bannerAd_;
    }
    nlohmann::json json;
    json["adId"] = adId;
    json["adSize"] = static_cast<int>(adSize);
    auto response = bridge_.call(kCreateBannerAd, json.dump());
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto size = getBannerAdSize(adSize);
    bannerAd_ = std::make_shared<ads::GuardedBannerAd>(
        std::make_shared<ads::DefaultBannerAd>(
            "IronSourceBannerAd", bridge_, logger_,
            [this, adId] { //
                destroyBannerAd(adId);
            },
            adId, size));
    return bannerAd_;
}

bool Self::destroyBannerAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (bannerAd_ == nullptr) {
        return false;
    }
    auto&& response = bridge_.call(kDestroyAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    bannerAd_.reset();
    return true;
}

std::shared_ptr<IFullScreenAd>
Self::createInterstitialAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedInterstitialAd_) {
        return sharedInterstitialAd_;
    }
    interstitialAd_ =
        std::make_shared<InterstitialAd>(logger_, displayer_, this, adId);
    sharedInterstitialAd_ =
        std::make_shared<ads::GuardedFullScreenAd>(interstitialAd_);
    return sharedInterstitialAd_;
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedInterstitialAd_ == nullptr) {
        return false;
    }
    interstitialAd_.reset();
    sharedInterstitialAd_.reset();
    return true;
}

std::shared_ptr<IFullScreenAd> Self::createRewardedAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedRewardedAd_) {
        return sharedRewardedAd_;
    }
    rewardedAd_ = std::make_shared<RewardedAd>(logger_, displayer_, this, adId);
    sharedRewardedAd_ = std::make_shared<ads::GuardedFullScreenAd>(rewardedAd_);
    return sharedRewardedAd_;
}

bool Self::destroyRewardedAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedRewardedAd_ == nullptr) {
        return false;
    }
    rewardedAd_.reset();
    sharedRewardedAd_.reset();
    return true;
}

bool Self::hasInterstitialAd() const {
    auto response = bridge_.call(kHasInterstitialAd);
    return core::toBool(response);
}

void Self::loadInterstitialAd() {
    bridge_.call(kLoadInterstitialAd);
}

void Self::showInterstitialAd(const std::string& adId) {
    bridge_.call(kShowInterstitialAd, adId);
}

bool Self::hasRewardedAd() const {
    auto response = bridge_.call(kHasRewardedAd);
    return core::toBool(response);
}

void Self::showRewardedAd(const std::string& adId) {
    bridge_.call(kShowRewardedAd, adId);
}

#pragma mark - Interstitial Ad Callbacks.

void Self::onInterstitialAdLoaded() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAd_) {
        interstitialAd_->onLoaded();
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdFailedToLoad(const std::string& message) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAd_) {
        interstitialAd_->onFailedToLoad(message);
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdFailedToShow(const std::string& message) {
    if (interstitialAd_) {
        interstitialAd_->onFailedToShow(message);
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdClicked() {
    if (interstitialAd_) {
        interstitialAd_->onClicked();
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdClosed() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAd_) {
        interstitialAd_->onClosed();
    } else {
        onMediationAdClosed(FullScreenAdResult::Completed);
    }
}

#pragma mark - Rewarded Ad Callbacks.

void Self::onRewardedAdLoaded() {
    if (rewardedAd_) {
        rewardedAd_->onLoaded();
    } else {
        // Automatically reloaded by SDK.
    }
}

void Self::onRewardedAdFailedToShow(const std::string& message) {
    if (rewardedAd_) {
        rewardedAd_->onFailedToShow(message);
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClicked() {
    if (rewardedAd_) {
        rewardedAd_->onClicked();
    } else {
        assert(false);
    }
}

void Self::onRewardedAdClosed(bool rewarded) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (rewardedAd_) {
        rewardedAd_->onClosed(rewarded);
    } else {
        onMediationAdClosed(rewarded ? FullScreenAdResult::Completed
                                     : FullScreenAdResult::Canceled);
    }
}

#pragma mark - Mediation Ad Callbacks.

void Self::onMediationAdClosed(FullScreenAdResult result) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (displayer_->isProcessing()) {
        displayer_->resolve(result);
        return;
    }
    assert(false);
}
} // namespace iron_source
} // namespace ee
