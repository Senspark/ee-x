//
//  AppsFlyerBridge.cpp
//  ee_x_mobile_apps_flyer
//
//  Created by eps on 6/26/18.
//

#include "ee/apps_flyer/private/AppsFlyerBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace apps_flyer {
namespace {
// clang-format off
const std::string kPrefix   = "AppsFlyerBridge";
const auto kInitialize      = kPrefix + "Initialize";
const auto kStartTracking   = kPrefix + "StartTracking";
const auto kGetDeviceId     = kPrefix + "GetDeviceId";
const auto kSetDebugEnabled = kPrefix + "SetDebugEnabled";
const auto kSetStopTracking = kPrefix + "SetStopTracking";
const auto kTrackEvent      = kPrefix + "TrackEvent";
const auto kLogAdRevenue    = kPrefix + "LogAdRevenue";
const auto kOnPurchaseValidated    = kPrefix + "OnPurchaseValidated";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {}

Self::~Bridge() = default;

void Self::destroy() {
    destroyer_();
}

void Self::initialize(const std::string& devKey, const std::string& iosAppId) {
    if (initialized_) {
        return;
    }
    initialized_ = true;
    nlohmann::json json;
    json["devKey"] = devKey;
    json["iosAppId"] = iosAppId;
    bridge_.call(kInitialize, json.dump());
}

void Self::startTracking() {
    bridge_.call(kStartTracking);
}

std::string Self::getDeviceId() const {
    return bridge_.call(kGetDeviceId);
}

void Self::setDebugEnabled(bool enabled) {
    bridge_.call(kSetDebugEnabled, core::toString(enabled));
}

void Self::setStopTracking(bool enabled) {
    bridge_.call(kSetStopTracking, core::toString(enabled));
}

void Self::trackEvent(const std::string& name,
                      const std::map<std::string, std::string>& values) {
    nlohmann::json json;
    json["name"] = name;
    json["values"] = values;
    bridge_.call(kTrackEvent, json.dump());
}

void Self::logRevenue(const ILibraryAnalytics::AdRevenue &adRevenue) {
    if (!adRevenue.isValid()) {
        return;
    }

    std::string mediationNetwork;
    switch (adRevenue.mediationNetwork) {
        case AdNetwork::AdMob:
            mediationNetwork = "admob";
            break;
        case AdNetwork::AppLovin:
            mediationNetwork = "applovin";
            break;
        default:
            mediationNetwork = "others";
            break;
    }

    std::string adFormat;
    switch (adRevenue.adFormat) {
        case AdFormat::AppOpen:
            adFormat = "App Open";
            break;
        case AdFormat::Banner:
            adFormat = "Banner";
            break;
        case AdFormat::Interstitial:
            adFormat = "Interstitial";
            break;
        case AdFormat::Rectangle:
            adFormat = "Rect";
            break;
        case AdFormat::Rewarded:
            adFormat = "Rewarded";
            break;
        case AdFormat::RewardedInterstitial:
            adFormat = "Rewarded Interstitial";
            break;
        default:
            adFormat = "Others";
            break;
    }

    nlohmann::json json;
    json["mediationNetwork"] = mediationNetwork;
    json["monetizationNetwork"] = adRevenue.monetizationNetwork;
    json["revenue"] = adRevenue.revenue;
    json["currencyCode"] = adRevenue.currencyCode;
    json["adFormat"] = adFormat;
    json["adUnit"] = adRevenue.adUnit;

    auto output = json.dump();
    logger_.info("%s: %s", kPrefix.c_str(), output.c_str());
    bridge_.call(kLogAdRevenue, output);
}

void Self::logRevenue(const ILibraryAnalytics::IapRevenue &iapRevenue) {
    // Đã sử dụng thư viện track iap của AppsFlyer nên ko gửi price nữa
    nlohmann::json json;
    json["af_currency"] = iapRevenue.currencyCode;
    json["af_revenue"] = 0;
    json["af_quantity"] = 1;
    json["af_content_type"] = "iap_product";
    json["af_content_id"] = iapRevenue.productId;

    auto output = json.dump();
    logger_.info("%s: %s", kPrefix.c_str(), output.c_str());
    bridge_.call(kTrackEvent, output);
}
} // namespace apps_flyer
} // namespace ee
