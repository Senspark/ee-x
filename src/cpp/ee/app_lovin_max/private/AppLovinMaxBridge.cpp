#include "ee/app_lovin_max/private/AppLovinMaxBridge.hpp"

#include <ee/ads/internal/Capper.hpp>
#include <ee/ads/internal/DefaultBannerAd.hpp>
#include <ee/ads/internal/GuardedBannerAd.hpp>
#include <ee/ads/internal/GuardedFullScreenAd.hpp>
#include <ee/ads/internal/IAsyncHelper.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/ads/internal/Retrier.hpp>
#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/ILibraryAnalytics.h>
#include <ee/nlohmann/json.hpp>

#include "ee/app_lovin_max/private/AppLovinMaxInterstitialAd.hpp"
#include "ee/app_lovin_max/private/AppLovinMaxRewardedAd.hpp"
#include "ee/app_lovin_max/private/AppLovinMaxBannerAd.hpp"

namespace ee {
namespace app_lovin_max {
namespace {
const std::string kPrefix = "AppLovinMaxBridge";
const auto kInitialize = kPrefix + "Initialize";
const auto kGetBannerAdSize = kPrefix + "GetBannerAdSize";
const auto kCreateBannerAd = kPrefix + "CreateBannerAd";
const auto kOnBannerAdPaid = kPrefix + "OnBannerAdPaid";
const auto kDestroyAd = kPrefix + "DestroyAd";
const auto kHasInterstitialAd = kPrefix + "HasInterstitialAd";
const auto kLoadInterstitialAd = kPrefix + "LoadInterstitialAd";
const auto kShowInterstitialAd = kPrefix + "ShowInterstitialAd";
const auto kHasRewardedAd = kPrefix + "HasRewardedAd";
const auto kLoadRewardedAd = kPrefix + "LoadRewardedAd";
const auto kShowRewardedAd = kPrefix + "ShowRewardedAd";
const auto kOnInterstitialAdLoaded = kPrefix + "OnInterstitialAdLoaded";
const auto kOnInterstitialAdFailedToLoad =
    kPrefix + "OnInterstitialAdFailedToLoad";
const auto kOnInterstitialAdFailedToShow =
    kPrefix + "OnInterstitialAdFailedToShow";
const auto kOnInterstitialAdClicked = kPrefix + "OnInterstitialAdClicked";
const auto kOnInterstitialAdClosed = kPrefix + "OnInterstitialAdClosed";
const auto kOnInterstitialAdPaid = kPrefix + "OnInterstitialAdPaid";
const auto kOnRewardedAdLoaded = kPrefix + "OnRewardedAdLoaded";
const auto kOnRewardedAdFailedToLoaded = kPrefix + "OnRewardedAdFailedToLoad";
const auto kOnRewardedAdFailedToShow = kPrefix + "OnRewardedAdFailedToShow";
const auto kOnRewardedAdClicked = kPrefix + "OnRewardedAdClicked";
const auto kOnRewardedAdClosed = kPrefix + "OnRewardedAdClosed";
const auto kOnRewardedAdPaid = kPrefix + "OnRewardedAdPaid";
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer)
    , network_("max") {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    displayer_ = mediation.getAdDisplayer();
    interstitialAd_ = nullptr;
    rewardedAd_ = nullptr;

    bridge_.registerHandler(
        [this](const std::string &message) {//
            onBannerAdPaid(message);
        },
        kOnBannerAdPaid);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onInterstitialAdLoaded();
        },
        kOnInterstitialAdLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onInterstitialAdFailedToLoad(json["code"], json["message"]);
        },
        kOnInterstitialAdFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onInterstitialAdFailedToShow(json["code"], json["message"]);
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
        [this](const std::string &message) {//
            onInterstitialAdPaid(message);
        },
        kOnInterstitialAdPaid);
    bridge_.registerHandler(
        [this](const std::string& message) { //
            onRewardedAdLoaded();
        },
        kOnRewardedAdLoaded);
    bridge_.registerHandler(
            [this](const std::string& message) {
                auto json = nlohmann::json::parse(message);
                onRewardedAdFailedToLoad(json["code"], json["message"]);
            },
            kOnRewardedAdFailedToLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onRewardedAdFailedToShow(json["code"], json["message"]);
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
    bridge_.registerHandler(
        [this](const std::string &message) {//
            onRewardedAdPaid(message);
        },
        kOnRewardedAdPaid);
    }

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);

    bridge_.deregisterHandler(kOnInterstitialAdLoaded);
    bridge_.deregisterHandler(kOnInterstitialAdFailedToLoad);
    bridge_.deregisterHandler(kOnInterstitialAdFailedToShow);
    bridge_.deregisterHandler(kOnInterstitialAdClicked);
    bridge_.deregisterHandler(kOnInterstitialAdClosed);
    bridge_.deregisterHandler(kOnInterstitialAdPaid);

    bridge_.deregisterHandler(kOnRewardedAdLoaded);
    bridge_.deregisterHandler(kOnRewardedAdFailedToShow);
    bridge_.deregisterHandler(kOnRewardedAdClicked);
    bridge_.deregisterHandler(kOnRewardedAdClosed);
    bridge_.deregisterHandler(kOnRewardedAdPaid);

    destroyer_();
}

Task<bool> Self::initialize(const std::string& bannerId,
                            const std::string& rewardedId,
                            const std::string& interstitialId) {
    logger_.debug("%s: bannerId = %s interstitialId = %s rewardedId = %s",
                  __PRETTY_FUNCTION__, bannerId.c_str(), interstitialId.c_str(),
                  rewardedId.c_str());
    nlohmann::json json;
    json["bannerAdId"] = bannerId;
    json["interstitialAdId"] = interstitialId;
    json["rewardedAdId"] = rewardedId;
    auto response = co_await bridge_.callAsync(kInitialize, json.dump());
    createInterstitialAd(interstitialId);
    createRewardedAd(rewardedId);
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
    if (sharedBannerAd_) {
        return sharedBannerAd_;
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

    bannerAd_ = std::make_shared<BannerAd>( //
        kPrefix, bridge_, logger_,
        [this, adId] { //
            destroyBannerAd(adId);
        }, network_, adId, size);

    sharedBannerAd_ = std::make_shared<ads::GuardedBannerAd>(bannerAd_);
    return sharedBannerAd_;
}

void Self::onBannerAdPaid(const std::string& jsonStr) {
    if (bannerAd_) {
        onAdPaid(AdFormat::Banner, jsonStr);
    }
}

bool Self::destroyBannerAd(const std::string& adId) {
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedBannerAd_ == nullptr) {
        return false;
    }
    auto&& response = bridge_.call(kDestroyAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    sharedBannerAd_.reset();
    return true;
}

std::shared_ptr<IFullScreenAd>
Self::createInterstitialAd(const std::string& adId) {
    // adId has no usage at the moment since all ads share the same instance.
    logger_.debug("%s: adId = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (sharedInterstitialAd_) {
        return sharedInterstitialAd_;
    }
    interstitialAd_ = std::make_shared<InterstitialAd>(logger_, displayer_,
                                                       this, adId);
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

Task<bool> Self::loadRewardedAd() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    auto response = co_await bridge_.callAsync(kLoadRewardedAd);
    co_return core::toBool(response);
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

void Self::onInterstitialAdFailedToLoad(int code, const std::string& message) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (interstitialAd_) {
        interstitialAd_->onFailedToLoad(code, message);
    } else {
        assert(false);
    }
}

void Self::onInterstitialAdFailedToShow(int code, const std::string& message) {
    if (interstitialAd_) {
        interstitialAd_->onFailedToShow(code, message);
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
        onMediationAdClosed(AdResult::Completed);
    }
}

void Self::onInterstitialAdPaid(const std::string& jsonStr) {
    if (interstitialAd_) {
        onAdPaid(AdFormat::Interstitial, jsonStr);
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

void Self::onRewardedAdFailedToLoad(int code, const std::string& message) {
    if (rewardedAd_) {
        rewardedAd_->onFailedToLoad(code, message);
    } else {
        assert(false);
    }
}

void Self::onRewardedAdFailedToShow(int code, const std::string& message) {
    if (rewardedAd_) {
        rewardedAd_->onFailedToShow(code, message);
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
    if (rewardedAd_) {
        rewardedAd_->onClosed(rewarded);
    } else {
        onMediationAdClosed(rewarded ? AdResult::Completed
                                     : AdResult::Canceled);
    }
}

void Self::onRewardedAdPaid(const std::string& jsonStr) {
    if (rewardedAd_) {
        onAdPaid(AdFormat::Rewarded, jsonStr);
    }
}

void Self::onAdPaid(AdFormat adFormat, const std::string &jsonStr) {
    logger_.debug("%s: jsonData = %s", __PRETTY_FUNCTION__, jsonStr.c_str());
    if (!analytics_) {
        return;
    }
    try {
        auto json = nlohmann::json::parse(jsonStr);
        auto adSource = json["networkName"].get<std::string>();
        auto revenue = json["revenue"].get<double>();
        auto adUnitId = json["adUnitId"].get<std::string>();

        analytics_->logRevenue(ee::core::analytics::AdRevenue {
                .mediationNetwork = AdNetwork::AppLovinMax,
                .monetizationNetwork = adSource,
                .revenue = revenue,
                .currencyCode = "USD",
                .adFormat = adFormat,
                .adUnit = adUnitId
        });
    }
    catch (const std::exception &e) {
        logger_.error("%s: %s", __PRETTY_FUNCTION__, e.what());
    }
}

#pragma mark - Mediation Ad Callbacks.

void Self::onMediationAdClosed(AdResult result) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    if (displayer_->isProcessing()) {
        displayer_->resolve(result);
        return;
    }
    assert(false);
}

void Bridge::addAnalytics(std::shared_ptr<ILibraryAnalytics> analytics) {
    analytics_ = analytics;
}
    } // namespace app_lovin_max
} // namespace ee