//
//  FirebaseAnalytics.cpp
//  ee_x
//
//  Created by Zinge on 5/10/17.
//
//

#include "ee/cost_center_analytics/internal/CostCenterAnalyticsBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/JsonUtils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee::cost_center::analytics {
    namespace {
        // Dùng Firebase để Log event
        const std::string kPrefix = "FirebaseAnalyticsBridge";
        const std::string kLogEvent = kPrefix + "LogEvent";
    } // namespace

    using Self = Bridge;

    Bridge::Bridge(IMessageBridge &bridge, ILogger &logger, const Destroyer &destroyer) :
        bridge_(bridge),
        logger_(logger),
        destroyer_(destroyer) {}

    Self::~Bridge() = default;

    void Self::destroy() {
        destroyer_();
    }

    Task<bool> Self::initialize() {
        co_return true;
    }

    void Self::logRevenue(const ILibraryAnalytics::AdRevenue &adRevenue) {
        // Log ad revenue dùng Firebase (khác với ad_impression đã log bên Firebase)

        std::string adFormat;
        switch (adRevenue.adFormat) {
            case AdFormat::AppOpen:
                adFormat = "app_open";
                break;
            case AdFormat::Banner:
                adFormat = "banner";
                break;
            case AdFormat::Interstitial:
                adFormat = "inter";
                break;
            case AdFormat::Rectangle:
                adFormat = "banner_rect";
                break;
            case AdFormat::Rewarded:
                adFormat = "reward";
                break;
            case AdFormat::RewardedInterstitial:
                adFormat = "reward_inter";
                break;
            default:
                adFormat = "others";
                break;
        }

        nlohmann::json request;
        request["name"] = "ad_revenue_sdk";
        request["parameters"] = std::unordered_map<std::string, std::variant<std::int64_t, double, std::string>>{
            {"ad_format",   adFormat},
            {"value",       adRevenue.revenue},
            {"currency",    adRevenue.currencyCode},
        };
        bridge_.call(kLogEvent, request.dump());
    }

    void Self::logRevenue(const ILibraryAnalytics::IapRevenue &iapRevenue) {
        // not yet implemented
    }
} // namespace ee
