//
//  StoreSubscriptionInfo.cpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#include "ee/store/StoreSubscriptionInfo.hpp"

#include <ee/core/Logger.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/store/StoreAppleInAppPurchaseReceipt.hpp"
#include "ee/store/StoreAppleStoreProductType.hpp"
#include "ee/store/StoreResult.hpp"
#include "ee/store/StoreSubscriptionPeriodUnit.hpp"

namespace ee {
namespace store {
using Self = SubscriptionInfo;

Self::SubscriptionInfo(
    const std::shared_ptr<AppleInAppPurchaseReceipt>& receipt,
    const std::string& introJson)
    : logger_(Logger::getSystemLogger()) {
    assert(receipt->productType() != AppleStoreProductType::Consumable &&
           receipt->productType() != AppleStoreProductType::NonConsumable);
    if (not introJson.empty()) {
        auto dic = nlohmann::json::parse(introJson);
        int numberOfUnits = -1;
        auto subscriptionPeriodUnit = SubscriptionPeriodUnit::NotAvailable;
        introductoryPrice_ = dic["introductoryPrice"].get<std::string>() +
                             dic["introductoryPriceLocale"].get<std::string>();
        if (introductoryPrice_.empty()) {
            introductoryPrice_ = "not available";
        } else {
            try {
                introductoryPriceCycles_ =
                    dic["introductoryPriceNumberOfPeriods"];
                numberOfUnits = dic["numberOfUnits"];
                subscriptionPeriodUnit = dic["unit"];
            } catch (const std::exception& ex) {
                logger_.info("%s: Unable to parse introductory period cycles "
                             "and duration, this product does not have "
                             "configuration of introductory price period %s",
                             __PRETTY_FUNCTION__, ex.what());
                subscriptionPeriodUnit = SubscriptionPeriodUnit::NotAvailable;
            }
        }
        switch (subscriptionPeriodUnit) {
        case SubscriptionPeriodUnit::Day:
            introductoryPricePeriod_ = 86400 * numberOfUnits;
            break;
        case SubscriptionPeriodUnit::Month:
            introductoryPricePeriod_ = 86400 * 30 * numberOfUnits;
            break;
        case SubscriptionPeriodUnit::Week:
            introductoryPricePeriod_ = 86400 * 7 * numberOfUnits;
            break;
        case SubscriptionPeriodUnit::Year:
            introductoryPricePeriod_ = 86400 * 365 * numberOfUnits;
            break;
        case SubscriptionPeriodUnit::NotAvailable:
            introductoryPricePeriod_ = 0;
            introductoryPriceCycles_ = 0;
            break;
        }
    } else {
        introductoryPrice_ = "not available";
        introductoryPricePeriod_ = 0;
        introductoryPriceCycles_ = 0;
    }
    purchaseDate_ = receipt->purchaseDate();
    productId_ = receipt->productId();
    subscriptionExpireDate_ = receipt->subscriptionExpirationDate();
    subscriptionCancelDate_ = receipt->cancellationDate();
    auto utcNow =
        static_cast<int>(std::chrono::time_point_cast<std::chrono::seconds>(
                             std::chrono::system_clock::now())
                             .time_since_epoch()
                             .count());
    if (receipt->productType() ==
        AppleStoreProductType::NonRenewingSubscription) {
        isSubscribed_ = Result::Unsupported;
        isExpired_ = Result::Unsupported;
        isCancelled_ = Result::Unsupported;
        isFreeTrial_ = Result::Unsupported;
        isAutoRenewing_ = Result::Unsupported;
        isIntroductoryPricePeriod_ = Result::Unsupported;
    } else {
        if (receipt->cancellationDate() > 0 &&
            receipt->cancellationDate() < utcNow) {
            isCancelled_ = Result::True;
        } else {
            isCancelled_ = Result::False;
        }
        if (receipt->subscriptionExpirationDate() >= utcNow) {
            isSubscribed_ = Result::True;
        } else {
            isSubscribed_ = Result::False;
        }
        if (receipt->subscriptionExpirationDate() > 0 &&
            receipt->subscriptionExpirationDate() < utcNow) {
            isExpired_ = Result::True;
        } else {
            isExpired_ = Result::False;
        }
        if (receipt->isFreeTrial()) {
            isFreeTrial_ = Result::True;
        } else {
            isFreeTrial_ = Result::False;
        }
        if (isCancelled_ == Result::True && isExpired_ == Result::True) {
            isAutoRenewing_ = Result::True;
        } else {
            isAutoRenewing_ = Result::False;
        }
        if (receipt->isIntroductoryPricePeriod()) {
            isIntroductoryPricePeriod_ = Result::True;
        } else {
            isIntroductoryPricePeriod_ = Result::False;
        }
    }
    if (isSubscribed_ == Result::True) {
        remainedTime_ = receipt->subscriptionExpirationDate() - utcNow;
    } else {
        remainedTime_ = 0;
    }

    // Google only.
    freeTrialPeriod_ = 0;
    subscriptionPeriod_ = 0;
}

const std::string& Self::getProductId() const {
    return productId_;
}

int Self::getPurchaseDate() const {
    return purchaseDate_;
}

Result Self::isSubscribed() const {
    return isSubscribed_;
}

Result Self::isExpired() const {
    return isExpired_;
}

Result Self::isCancelled() const {
    return isCancelled_;
}

Result Self::isFreeTrial() const {
    return isFreeTrial_;
}

Result Self::isAutoRenewing() const {
    return isAutoRenewing_;
}

int Self::getRemainingTime() const {
    return remainedTime_;
}

Result Self::isIntroductoryPricePeriod() const {
    return isIntroductoryPricePeriod_;
}

int Self::getIntroductoryPricePeriod() const {
    return introductoryPricePeriod_;
}

const std::string& Self::getIntroductoryPrice() const {
    return introductoryPrice_;
}

int Self::getIntroductoryPricePeriodCycles() const {
    return introductoryPriceCycles_;
}

int Self::getExpireDate() const {
    return subscriptionExpireDate_;
}

int Self::getCancelDate() const {
    return subscriptionCancelDate_;
}

int Self::getFreeTrialPeriod() const {
    return freeTrialPeriod_;
}

int Self::getSubscriptionPeriod() const {
    return subscriptionPeriod_;
}

const std::string& Self::getFreeTrialPeriodString() const {
    return freeTrialPeriodString_;
}

const std::string& Self::getSkuDetails() const {
    return skuDetails_;
}
} // namespace store
} // namespace ee
