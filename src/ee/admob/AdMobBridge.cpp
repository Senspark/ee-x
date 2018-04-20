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
#include "ee/core/Logger.hpp"
#include "ee/core/MessageBridge.hpp"
#include "ee/core/Utils.hpp"

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

Self::AdMob()
    : bridge_(MessageBridge::getInstance()) {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    loading_ = false;
    rewarded_ = false;

    bridge_.registerHandler(
        [this](const std::string& message) {
            onReward();
            return "";
        },
        k__onRewarded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onFailedToLoad(message);
            return "";
        },
        k__onFailedToLoad);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onLoaded();
            return "";
        },
        k__onLoaded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onOpened();
            return "";
        },
        k__onOpened);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onClosed();
            return "";
        },
        k__onClosed);
}

Self::~AdMob() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    bridge_.deregisterHandler(k__onRewarded);
    bridge_.deregisterHandler(k__onFailedToLoad);
    bridge_.deregisterHandler(k__onLoaded);
    bridge_.deregisterHandler(k__onOpened);
    bridge_.deregisterHandler(k__onClosed);
}

void Self::initialize(const std::string& applicationId) {
    bridge_.call(k__initialize, applicationId);
}

std::string Self::getEmulatorTestDeviceHash() const {
    return bridge_.call(k__getEmulatorTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    bridge_.call(k__addTestDevice, hash);
}

std::shared_ptr<IAdView> Self::createBannerAd(const std::string& adId,
                                                      BannerAdSize adSize) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__ad_size] = static_cast<int>(adSize);

    auto response = bridge_.call(k__createBannerAd, json.dump());
    if (not core::toBool(response)) {
        return nullptr;
    }
    return std::shared_ptr<IAdView>(new BannerAd(bridge_, this, adId));
}

bool Self::destroyBannerAd(const std::string& adId) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    auto response = bridge_.call(k__destroyBannerAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IAdView>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__layout_name] = layoutName;
    json[k__identifiers] = identifiers.params_;

    auto&& response = bridge_.call(k__createNativeAd, json.dump());
    if (not core::toBool(response)) {
        return std::make_shared<NullAdView>();
    }
    return std::shared_ptr<IAdView>(new NativeAd(bridge_, this, adId));
}

bool Self::destroyNativeAd(const std::string& adId) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    auto&& response = bridge_.call(k__destroyNativeAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    auto response = bridge_.call(k__createInterstitialAd, adId);
    if (not core::toBool(response)) {
        return std::make_shared<NullInterstitialAd>();
    }
    return std::shared_ptr<IInterstitialAd>(
        new InterstitialAd(bridge_, this, adId));
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    auto&& response = bridge_.call(k__destroyInterstitialAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IRewardedVideo>
Self::createRewardedVideo(const std::string& adId) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    if (rewardedVideos_.count(adId) != 0) {
        return std::make_shared<NullRewardedVideo>();
    }
    auto result = new RewardedVideo(this, adId);
    rewardedVideos_[adId] = result;
    return std::shared_ptr<IRewardedVideo>(result);
}

bool Self::destroyRewardedVideo(const std::string& adId) {
    Logger::getSystemLogger().debug("%s: id = %s", __PRETTY_FUNCTION__,
                                    adId.c_str());
    if (rewardedVideos_.count(adId) == 0) {
        return false;
    }
    rewardedVideos_.erase(adId);
    return true;
}

bool Self::hasRewardedVideo() const {
    auto response = bridge_.call(k__hasRewardedVideo);
    return core::toBool(response);
}

void Self::loadRewardedVideo(const std::string& adId) {
    if (loading_) {
        return;
    }
    loading_ = true;
    currentId_ = adId;
    bridge_.call(k__loadRewardedVideo, adId);
}

bool Self::showRewardedVideo() {
    if (not hasRewardedVideo()) {
        return false;
    }
    rewarded_ = false;
    bridge_.call(k__showRewardedVideo);
    return true;
}

void Self::onLoaded() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    loading_ = false;
}

void Self::onFailedToLoad(const std::string& message) {
    Logger::getSystemLogger().debug("%s: message = %s", __PRETTY_FUNCTION__,
                                    message.c_str());
    loading_ = false;
}

void Self::onReward() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    rewarded_ = true;
}

void Self::onOpened() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    rewarded_ = false;
}

void Self::onClosed() {
    Logger::getSystemLogger().debug("%s", __PRETTY_FUNCTION__);
    auto&& mediation = ads::MediationManager::getInstance();
    if (rewardedVideos_.count(currentId_)) {
        currentId_.clear();
        auto successful = mediation.finishRewardedVideo(rewarded_);        
        assert(successful);
        return;
    }
}
} // namespace admob
} // namespace ee
