//
//  AdMob.cpp
//  ee_x
//
//  Created by Zinge on 10/11/17.
//
//

#include "ee/ad_mob/private/AdMobBridge.hpp"

#include <ee/ads/internal/DefaultBannerAd.hpp>
#include <ee/ads/internal/DefaultFullScreenAd.hpp>
#include <ee/ads/internal/GuardedBannerAd.hpp>
#include <ee/ads/internal/GuardedFullScreenAd.hpp>
#include <ee/ads/internal/MediationManager.hpp>
#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/ad_mob/AdMobNativeAdLayout.hpp"

namespace ee {
namespace ad_mob {
namespace {
// clang-format off
const std::string kPrefix                = "AdMobBridge";
const auto kInitialize                   = kPrefix + "Initialize";
const auto kGetEmulatorTestDeviceHash    = kPrefix + "GetEmulatorTestDeviceHash";
const auto kAddTestDevice                = kPrefix + "AddTestDevice";
const auto kGetBannerAdSize              = kPrefix + "GetBannerAdSize";
const auto kCreateBannerAd               = kPrefix + "CreateBannerAd";
const auto kCreateNativeAd               = kPrefix + "CreateNativeAd";
const auto kCreateAppOpenAd              = kPrefix + "CreateAppOpenAd";
const auto kCreateInterstitialAd         = kPrefix + "CreateInterstitialAd";
const auto kCreateRewardedInterstitialAd = kPrefix + "CreateRewardedInterstitialAd";
const auto kCreateRewardedAd             = kPrefix + "CreateRewardedAd";
const auto kDestroyAd                    = kPrefix + "DestroyAd";
// clang-format on
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
}

Self::~Bridge() = default;

void Self::destroy() {
    logger_.debug("%s", __PRETTY_FUNCTION__);
    for (auto&& [key, value] : ads_) {
        value->destroy();
    }
    destroyer_();
}

Task<bool> Self::initialize() {
    auto response = co_await bridge_.callAsync(kInitialize);
    co_return core::toBool(response);
}

std::string Self::getEmulatorTestDeviceHash() const {
    return bridge_.call(kGetEmulatorTestDeviceHash);
}

void Self::addTestDevice(const std::string& hash) {
    bridge_.call(kAddTestDevice, hash);
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
    auto iter = ads_.find(adId);
    if (iter != ads_.cend()) {
        return std::dynamic_pointer_cast<IBannerAd>(iter->second);
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
    auto ad = std::make_shared<ads::GuardedBannerAd>(
        std::make_shared<ads::DefaultBannerAd>(
            "AdMobBannerAd", bridge_, logger_,
            [this, adId] { //
                destroyAd(adId);
            },
            adId, size));
    ads_.emplace(adId, ad);
    return ad;
}

std::shared_ptr<IBannerAd>
Self::createNativeAd(const std::string& adId, const std::string& layoutName,
                     const NativeAdLayout& identifiers) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter != ads_.cend()) {
        return std::dynamic_pointer_cast<IBannerAd>(iter->second);
    }
    nlohmann::json json;
    json["adId"] = adId;
    json["layoutName"] = layoutName;
    json["identifiers"] = identifiers.params_;
    auto&& response = bridge_.call(kCreateNativeAd, json.dump());
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto ad = std::make_shared<ads::GuardedBannerAd>(
        std::make_shared<ads::DefaultBannerAd>(
            "AdMobNativeAd", bridge_, logger_,
            [this, adId] { //
                destroyAd(adId);
            },
            adId, std::pair(0, 0)));
    ads_.emplace(adId, ad);
    return ad;
}

std::shared_ptr<IFullScreenAd> Self::createAppOpenAd(const std::string& adId) {
    return createFullScreenAd(kCreateAppOpenAd, adId, [this, adId] {
        return std::make_shared<ads::DefaultFullScreenAd>(
            "AdMobAppOpenAd", bridge_, logger_, displayer_,
            [this, adId] { //
                return destroyAd(adId);
            },
            [](const std::string& message) { //
                return FullScreenAdResult::Completed;
            },
            adId);
    });
}

std::shared_ptr<IFullScreenAd>
Self::createInterstitialAd(const std::string& adId) {
    return createFullScreenAd(kCreateInterstitialAd, adId, [this, adId] {
        return std::make_shared<ads::DefaultFullScreenAd>(
            "AdMobInterstitialAd", bridge_, logger_, displayer_,
            [this, adId] { //
                return destroyAd(adId);
            },
            [](const std::string& message) { //
                return FullScreenAdResult::Completed;
            },
            adId);
    });
}

std::shared_ptr<IFullScreenAd>
Self::createRewardedInterstitialAd(const std::string& adId) {
    return createFullScreenAd(
        kCreateRewardedInterstitialAd, adId, [this, adId] {
            return std::make_shared<ads::DefaultFullScreenAd>(
                "AdMobRewardedInterstitialAd", bridge_, logger_, displayer_,
                [this, adId] { //
                    return destroyAd(adId);
                },
                [](const std::string& message) { //
                    return core::toBool(message) ? FullScreenAdResult::Completed
                                                 : FullScreenAdResult::Canceled;
                },
                adId);
        });
}

std::shared_ptr<IFullScreenAd> Self::createRewardedAd(const std::string& adId) {
    return createFullScreenAd(kCreateRewardedAd, adId, [this, adId] {
        return std::make_shared<ads::DefaultFullScreenAd>(
            "AdMobRewardedAd", bridge_, logger_, displayer_,
            [this, adId] { //
                return destroyAd(adId);
            },
            [](const std::string& message) { //
                return core::toBool(message) ? FullScreenAdResult::Completed
                                             : FullScreenAdResult::Canceled;
            },
            adId);
    });
}

std::shared_ptr<IFullScreenAd> Self::createFullScreenAd(
    const std::string& handlerId, const std::string& adId,
    const std::function<std::shared_ptr<IFullScreenAd>()>& creator) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter != ads_.cend()) {
        return std::dynamic_pointer_cast<IFullScreenAd>(iter->second);
    }
    auto response = bridge_.call(handlerId, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to create an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return nullptr;
    }
    auto ad = std::make_shared<ads::GuardedFullScreenAd>(creator());
    ads_.emplace(adId, ad);
    return ad;
}

bool Self::destroyAd(const std::string& adId) {
    logger_.debug("%s: id = %s", __PRETTY_FUNCTION__, adId.c_str());
    auto iter = ads_.find(adId);
    if (iter == ads_.cend()) {
        return false;
    }
    auto&& response = bridge_.call(kDestroyAd, adId);
    if (not core::toBool(response)) {
        logger_.error("%s: There was an error when attempt to destroy an ad.",
                      __PRETTY_FUNCTION__);
        assert(false);
        return false;
    }
    ads_.erase(iter);
    return true;
}
} // namespace ad_mob
} // namespace ee
