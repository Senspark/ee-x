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
        const std::string kTag = "CostCenterAnalyticsBridge";
        const std::string kFirebasePrefix = "FirebaseAnalyticsBridge";
        const std::string kAppsFlyerPrefix = "AppsFlyerBridge";
        const std::string kLogEvent = kFirebasePrefix + "LogEvent";
        const std::string kOnPurchaseValidated = kAppsFlyerPrefix + "OnPurchaseValidated";
    } // namespace

    using Self = Bridge;

    Bridge::Bridge(IMessageBridge &bridge, ILogger &logger, const Destroyer &destroyer) :
        bridge_(bridge),
        logger_(logger),
        destroyer_(destroyer) {
    }

    Self::~Bridge() = default;

    void Self::destroy() {
        destroyer_();
        bridge_.deregisterHandler(kOnPurchaseValidated);
    }

    Task<bool> Self::initialize() {
        bridge_.registerHandler([this](const std::string &message) {
            onPurchaseValidated(message);
        }, kOnPurchaseValidated);
        co_return true;
    }

    void Self::logRevenue(const ILibraryAnalytics::AdRevenue &adRevenue) {
        // Log ad revenue dùng Firebase (khác với ad_impression đã log bên Firebase)

        if (!adRevenue.isValid()) {
            return;
        }

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
            {"ad_format", adFormat},
            {"value",     adRevenue.revenue},
            {"currency",  adRevenue.currencyCode},
        };

        auto output = request.dump();
        logger_.info("%s: %s", kTag.c_str(), output.c_str());
        bridge_.call(kLogEvent, output);
    }

    void Self::logRevenue(const ILibraryAnalytics::IapRevenue &iapRevenue) {
        // Chờ kết quả từ AppsFlyer
        lastIapRevenue_ = std::make_unique<IapRevenue>(std::move(iapRevenue));
        logger_.info("%s: Add to pending %s %s", kTag.c_str(), iapRevenue.productId.c_str(),
                     iapRevenue.orderId.c_str());
    }

    void Bridge::onPurchaseValidated(const std::string &jsonData) {
        try {
            auto json = nlohmann::json::parse(jsonData);
            auto isSuccess = json["isSuccess"].get<bool>();
            auto isTestPurchase = json["isTestPurchase"].get<bool>();
            auto orderId = json["orderId"].get<std::string>();
            auto productId = json["productId"].get<std::string>();

            if (lastIapRevenue_ == nullptr) {
                logger_.error("%s: lastIapRevenue_ is null", kTag.c_str());
                return;
            }
            if (lastIapRevenue_->orderId != orderId) {
                logger_.error("%s: orderId or productId doesn't match", kTag.c_str());
                return;
            }
            logIapRevenue(*lastIapRevenue_, isTestPurchase);
        } catch (const std::exception &e) {
            logger_.error("%s: %s", kTag.c_str(), e.what());
            return;
        }
    }

    void Bridge::logIapRevenue(const ILibraryAnalytics::IapRevenue &iapRevenue, bool isTestPurchase) {
        nlohmann::json request;
        request["name"] = isTestPurchase ? "iap_sdk_test" : "iap_sdk";
        request["parameters"] = std::unordered_map<std::string, std::variant<std::int64_t, double, std::string>>{
            {"product_id", iapRevenue.productId},
            {"order_id",   iapRevenue.orderId},
            {"value",      iapRevenue.revenue},
            {"currency",   iapRevenue.currencyCode},
        };

        auto output = request.dump();
        logger_.info("%s: %s", kTag.c_str(), output.c_str());
        bridge_.call(kLogEvent, output);
    }
} // namespace ee
