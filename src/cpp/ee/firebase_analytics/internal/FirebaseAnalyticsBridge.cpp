//
//  FirebaseAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/firebase_analytics/internal/FirebaseAnalyticsBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/JsonUtils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace firebase {
namespace analytics {
namespace {
const std::string kPrefix = "FirebaseAnalyticsBridge";
const std::string kInitialize = kPrefix + "Initialize";
const std::string kSetUserProperty = kPrefix + "SetUserProperty";
const std::string kTrackScreen = kPrefix + "TrackScreen";
const std::string kLogEvent = kPrefix + "LogEvent";
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

Task<bool> Self::initialize() {
    auto response = co_await bridge_.callAsync(kInitialize);
    co_return core::toBool(response);
}

void Self::setUserProperty(const std::string& key, const std::string& value) {
    nlohmann::json request;
    request["key"] = key;
    request["value"] = value;
    bridge_.call(kSetUserProperty, request.dump());
}

void Self::trackScreen(const std::string& name) {
    bridge_.call(kTrackScreen, name);
}

void Self::logEvent(
    const std::string& name,
    const std::unordered_map<std::string,
                             std::variant<std::int64_t, double, std::string>>&
        parameters) {
    nlohmann::json request;
    request["name"] = name;
    request["parameters"] = parameters;
    bridge_.call(kLogEvent, request.dump());
}

void Self::logRevenue(const ILibraryAnalytics::AdRevenue &adRevenue) {
    if (adRevenue.mediationNetwork == AdNetwork::AdMob) {
        // https://firebase.google.com/docs/analytics/measure-ad-revenue#implementation-admob
        // Logged automatically.
        return;
    }

    auto evName = adRevenue.isValid() ? "ad_impression" : "ad_impression_error";

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

    std::string adPlatform;
    switch (adRevenue.mediationNetwork) {
        case ads::AdNetwork::AppLovin:
            adPlatform = "AppLovin";
            break;
        case ads::AdNetwork::IronSource:
            adPlatform = "IronSource";
            break;
        default:
            adPlatform = "Others";
            break;
    }

    nlohmann::json request;
    request["name"] = evName;
    request["parameters"] = std::unordered_map<std::string, std::variant<std::int64_t, double, std::string>>{
        {"ad_platform", adPlatform},
        {"ad_source", adRevenue.monetizationNetwork},
        {"ad_unit", adRevenue.adUnit},
        {"ad_format", adFormat},
        {"value", adRevenue.revenue},
        {"currency", adRevenue.currencyCode},
    };
    bridge_.call(kLogEvent, request.dump());
}

void Self::logRevenue(const ILibraryAnalytics::IapRevenue &iapRevenue) {
    // do nothing
}
} // namespace analytics
} // namespace firebase
} // namespace ee
