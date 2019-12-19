//
//  AdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/admob/AdMobBridge.hpp"

#include <ee/nlohmann/json.hpp>

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
#include "ee/core/internal/SharedPtrUtils.hpp"

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

constexpr auto k__createRewardVideoAd       = "AdMob_createRewardVideoAd";
constexpr auto k__destroyRewardVideoAd      = "AdMob_destroyRewardVideoAd";
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
    : Self(Logger::getSystemLogger()) {}

Self::AdMob(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    loading_ = false;
}

Self::~AdMob() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
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
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
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
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto response = bridge_.call(k__destroyBannerAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IAdView>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    nlohmann::json json;
    json[k__ad_id] = adId;
    json[k__layout_name] = layoutName;
    json[k__identifiers] = identifiers.params_;

    auto&& response = bridge_.call(k__createNativeAd, json.dump());
    if (not core::toBool(response)) {
        return core::makeShared<NullAdView>();
    }
    return std::shared_ptr<IAdView>(new NativeAd(bridge_, logger_, this, adId));
}

bool Self::destroyNativeAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto&& response = bridge_.call(k__destroyNativeAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IInterstitialAd>
Self::createInterstitialAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto response = bridge_.call(k__createInterstitialAd, adId);
    if (not core::toBool(response)) {
        return core::makeShared<NullInterstitialAd>();
    }
    return std::shared_ptr<IInterstitialAd>(
        new InterstitialAd(bridge_, logger_, this, adId));
}

bool Self::destroyInterstitialAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto&& response = bridge_.call(k__destroyInterstitialAd, adId);
    return core::toBool(response);
}

std::shared_ptr<IRewardedVideo>
Self::createRewardedVideo(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto response = bridge_.call(k__createRewardVideoAd, adId);
    if (not core::toBool(response)) {
        return core::makeShared<NullRewardedVideo>(logger_);
    }

    auto result = new RewardedVideo(bridge_, logger_, this, adId);
    rewardedVideos_[adId] = result;
    return std::shared_ptr<IRewardedVideo>(result);
}

bool Self::destroyRewardedVideo(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    if (rewardedVideos_.count(adId) != 0) {
        rewardedVideos_.erase(adId);
    }

    auto&& response = bridge_.call(k__destroyRewardVideoAd, adId);
    return core::toBool(response);
}
} // namespace admob
} // namespace ee
