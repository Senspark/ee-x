//
// Created by nhanc18 on 24/10/2023.
//

#ifndef COCOS2DCPP_ILIBRARYANALYTICS_H
#define COCOS2DCPP_ILIBRARYANALYTICS_H

#ifdef __cplusplus

#include <unordered_map>
#include <variant>
#include <string>

#include <ee/core/IPlugin.hpp>
#include <ee/ads/AdNetwork.hpp>
#include <ee/ads/AdFormat.hpp>

#include "ee/firebase_analytics/FirebaseAnalyticsFwd.hpp"

namespace ee::core::analytics {
    struct AdRevenue {
        AdNetwork mediationNetwork;
        std::string monetizationNetwork;
        double revenue;
        std::string currencyCode;
        AdFormat adFormat;
        std::string adUnit;

    public:
        bool isValid() const {
            return !monetizationNetwork.empty() && !adUnit.empty() && revenue > 0;
        }
    };

    struct IapRevenue {
        double revenue;
        std::string currencyCode;
        std::string productId;
        std::string orderId;
    };
}

namespace ee::core {
    class ILibraryAnalytics {
    public:
        using AdRevenue = analytics::AdRevenue;
        using IapRevenue = analytics::IapRevenue;

        virtual void logRevenue(const AdRevenue &adRevenue) = 0;

        virtual void logRevenue(const IapRevenue &iapRevenue) = 0;

        virtual void pushGameLevel(int levelNo, const std::string& levelMode) = 0;

        virtual void popGameLevel(bool winGame) = 0;
    };
} // namespace ee

#endif // __cplusplus

#endif //COCOS2DCPP_ILIBRARYANALYTICS_H
