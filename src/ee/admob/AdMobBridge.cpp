//
//  AdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/admob/AdMobBridge.hpp"
#include "ee/admob/AdMobNativeAdLayout.hpp"
#include "ee/admob/internal/AdMobBannerAd.hpp"
#include "ee/admob/internal/AdMobInterstitialAd.hpp"
#include "ee/admob/internal/AdMobNativeAd.hpp"
#include "ee/admob/internal/AdMobRewardedVideo.hpp"
#include "ee/ads/NullAdView.hpp"
#include "ee/ads/NullInterstitialAd.hpp"
#include "ee/ads/NullRewardedVideo.hpp"
#include "ee/ads/internal/MediationManager.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/internal/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace admob {
using Self = AdMob;

namespace {
// clang-format off
constexpr auto k__initialize                = "AdMob_initialize";
    
constexpr auto k__getEmulatorTestDeviceHash = "AdMob_getEmulatorTestDeviceHash";
constexpr auto k__addTestDevice             = "AdMob_addTestDevice";
    
constexpr auto k__createBannerAd            = "AdMob_createBannerAd";
constexpr auto k__destroyBannerAd           = "AdMob_destroyBannerAd";

constexpr auto k__createNativeAd            = "AdMob_createNativeAd";
constexpr auto k__destroyNativeAd           = "AdMob_destroyNativeAd";

constexpr auto k__createInterstitialAd      = "AdMob_createInterstitialAd";
constexpr auto k__destroyInterstitialAd     = "AdMob_destroyInterstitialAd";

constexpr auto k__hasRewardedVideo          = "AdMob_hasRewardedVideo";
constexpr auto k__loadRewardedVideo         = "AdMob_loadRewardedVideo";
constexpr auto k__showRewardedVideo         = "AdMob_showRewardedVideo";

constexpr auto k__onRewarded                = "AdMob_onRewarded";
constexpr auto k__onFailedToLoad            = "AdMob_onFailedToLoad";
constexpr auto k__onLoaded                  = "AdMob_onLoaded";
constexpr auto k__onOpened                  = "AdMob_onOpened";
constexpr auto k__onClosed                  = "AdMob_onClosed";
// clang-format on
} // namespace

namespace {
// clang-format off
constexpr auto k__ad_id                 = "ad_id";
constexpr auto k__ad_size               = "ad_size";
constexpr auto k__layout_name           = "layout_name";
constexpr auto k__identifiers           = "identifiers";
// clang-format on
} // namespace

Self::AdMob() {
    loading_ = false;
    rewarded_ = false;

    auto&& bridge = core::MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            onReward();
            return "";
        },
        k__onRewarded);
    bridge.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad);
    bridge.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded);
    bridge.registerHandler(
        [this](const std::string& message) {
            onOpened();
            return "";
        },
        k__onOpened);
    bridge.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed);
}

Self::~AdMob() {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.deregisterHandler(k__onRewarded);
    bridge.deregisterHandler(k__onFailedToLoad);
    bridge.deregisterHandler(k__onLoaded);
    bridge.deregisterHandler(k__onOpened);
    bridge.deregisterHandler(k__onClosed);
}

void Self::initialize(const std::string& applicationId) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__initialize, applicationId);
}

std::string Self::getEmulatorTestDeviceHash() const {
    auto&& bridge = core::MessageBridge::getInstance();
    return bridge.call(k__getEmulatorTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__addTestDevice, hash);
}

std::shared_ptr<AdViewInterface> Self::createBannerAd(const std::string& adId,
                                                      BannerAdSize adSize) {
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__ad_size] = static_cast<int>(adSize);

    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createBannerAd, json.dump());
    if (not core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<AdViewInterface>(new BannerAd(this, adId));
}

bool Self::destroyBannerAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__destroyBannerAd, adId);
    return core::toBool(response);
}

std::shared_ptr<AdViewInterface>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__layout_name] = layoutName;
    json[k__identifiers] = identifiers.params_;

    auto&& bridge = core::MessageBridge::getInstance();
    auto&& response = bridge.call(k__createNativeAd, json.dump());
    if (not core::toBool(response)) {
        return std::make_shared<NullAdView>();
    }
    return std::shared_ptr<AdViewInterface>(new NativeAd(this, adId));
}

bool Self::destroyNativeAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto&& response = bridge.call(k__destroyNativeAd, adId);
    return core::toBool(response);
}

std::shared_ptr<InterstitialAdInterface>
Self::createInterstitialAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__createInterstitialAd, adId);
    if (not core::toBool(response)) {
        return std::make_shared<NullInterstitialAd>();
    }
    return std::shared_ptr<InterstitialAdInterface>(
        new InterstitialAd(this, adId));
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    auto&& bridge = core::MessageBridge::getInstance();
    auto&& response = bridge.call(k__destroyInterstitialAd, adId);
    return core::toBool(response);
}

std::shared_ptr<RewardedVideoInterface>
Self::createRewardedVideo(const std::string& adId) {
    if (rewardedVideos_.count(adId) != 0) {
        return std::make_shared<NullRewardedVideo>();
    }
    auto result = new RewardedVideo(this, adId);
    rewardedVideos_[adId] = result;
    return std::shared_ptr<RewardedVideoInterface>(result);
}

bool Self::destroyRewardedVideo(const std::string& adId) {
    if (rewardedVideos_.count(adId) == 0) {
        return false;
    }
    rewardedVideos_.erase(adId);
    return true;
}

bool Self::hasRewardedVideo() const {
    auto&& bridge = core::MessageBridge::getInstance();
    auto response = bridge.call(k__hasRewardedVideo);
    return core::toBool(response);
}

void Self::loadRewardedVideo(const std::string& adId) {
    if (loading_) {
        return;
    }
    loading_ = true;
    currentId_ = adId;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__loadRewardedVideo, adId);
}

bool Self::showRewardedVideo() {
    if (not hasRewardedVideo()) {
        return false;
    }
    rewarded_ = false;
    auto&& bridge = core::MessageBridge::getInstance();
    bridge.call(k__showRewardedVideo);
    return true;
}

void Self::onLoaded() {
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    loading_ = false;
}

void Self::onReward() {
    rewarded_ = true;
}

void Self::onOpened() {
    rewarded_ = false;
}

void Self::onClosed() {
    auto&& mediation = ads::MediationManager::getInstance();
    if (rewardedVideos_.count(currentId_)) {
        auto ad = rewardedVideos_.at(currentId_);
        ad->setResult(rewarded_);
        currentId_.clear();
        auto successful = mediation.finishRewardedVideo(ad);
        assert(successful);
        return;
    }

    // Other network mediation.
    auto successful = mediation.setRewardedVideoResult(rewarded_);
    assert(successful);
}
} // namespace admob
} // namespace ee
