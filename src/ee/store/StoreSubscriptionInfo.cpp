//
//  StoreSubscriptionInfo.cpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#include "ee/store/StoreSubscriptionInfo.hpp"

#include <ee/core/Logger.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/store/StoreResult.hpp"
#include "ee/store/private/StoreAppleInAppPurchaseReceipt.hpp"
#include "ee/store/private/StoreAppleStoreProductType.hpp"
#include "ee/store/private/StoreSubscriptionPeriodUnit.hpp"

namespace ee {
namespace store {
namespace {
int getUtcNow() {
    return static_cast<int>(std::chrono::time_point_cast<std::chrono::seconds>(
                                std::chrono::system_clock::now())
                                .time_since_epoch()
                                .count());
}

/// Parses ISO 8601 period.
int parsePeriodTimeUnits(const std::string& duration) {
    if (duration == "P1W") {
        return 86400 * 7;
    }
    if (duration == "P1M") {
        return 86400 * 30;
    }
    if (duration == "P3M") {
        return 86400 * 90;
    }
    if (duration == "P6M") {
        return 86400 * 180;
    }
    if (duration == "P1Y") {
        return 86400 * 365;
    }
    assert(false);
    return 0;
}

int nextBillingDate(int billingBeginDate, int units) {
    auto t1 = billingBeginDate;
    auto now = getUtcNow();
    while (t1 <= now) {
        t1 += units;
    }
    return t1;
}
} // namespace

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
    auto utcNow = getUtcNow();
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

Self::SubscriptionInfo(const std::string& skuDetails, bool isAutoRenewing,
                       int purchaseDate, bool isFreeTrial,
                       bool hasIntroductoryPriceTrial,
                       bool purchaseHistorySupported,
                       const std::string& updateMetadata)
    : logger_(Logger::getSystemLogger()) {
    auto dictionary = nlohmann::json::parse(skuDetails);
    if (dictionary["type"] == "inapp") {
        throw std::runtime_error("Invalid product type");
    }
    productId_ = dictionary["productId"];
    purchaseDate_ = purchaseDate;
    isSubscribed_ = Result::True;
    isAutoRenewing_ = isAutoRenewing ? Result::True : Result::False;
    isExpired_ = Result::False;
    isCancelled_ = isAutoRenewing ? Result::False : Result::True;
    isFreeTrial_ = Result::False;
    freeTrialPeriodString_ = dictionary.value("freeTrialPeriod", "");
    subscriptionPeriod_ =
        parsePeriodTimeUnits(dictionary.value("subscriptionPeriod", ""));
    freeTrialPeriod_ = 0;
    if (isFreeTrial) {
        freeTrialPeriod_ =
            parsePeriodTimeUnits(dictionary.value("freeTrialPeriod", ""));
    }
    introductoryPrice_ = dictionary.value("introductoryPrice", "");
    introductoryPriceCycles_ = dictionary.value("introductoryPriceCycles", 0);
    introductoryPricePeriod_ = 0;
    isIntroductoryPricePeriod_ = Result::False;
    int ts1 = 0;
    if (hasIntroductoryPriceTrial) {
        if (dictionary.contains("introductoryPricePeriod")) {
            introductoryPricePeriod_ =
                parsePeriodTimeUnits(dictionary["introductoryPricePeriod"]);
        } else {
            introductoryPricePeriod_ = subscriptionPeriod_;
        }
        ts1 = introductoryPricePeriod_ * introductoryPriceCycles_;
    }
    // FIXME.
    // Subscription update is not supported.
    assert(updateMetadata.empty());
    int ts2 = 0;
    auto utcNow = getUtcNow();
    auto timeSpan = utcNow - purchaseDate;
    if (timeSpan <= ts2) {
        subscriptionExpireDate_ = purchaseDate + ts2;
    } else if (timeSpan <= freeTrialPeriod_ + ts2) {
        isFreeTrial_ = Result::True;
        subscriptionExpireDate_ = purchaseDate + freeTrialPeriod_ + ts2;
    } else if (timeSpan < freeTrialPeriod_ + ts2 + ts1) {
        isIntroductoryPricePeriod_ = Result::True;
        subscriptionExpireDate_ = nextBillingDate(
            purchaseDate + freeTrialPeriod_ + ts2, introductoryPricePeriod_);
    } else {
        subscriptionExpireDate_ = nextBillingDate(
            purchaseDate + freeTrialPeriod_ + ts2 + ts1, subscriptionPeriod_);
    }
    remainedTime_ = subscriptionExpireDate_ - utcNow;
    skuDetails_ = skuDetails;
    if (purchaseHistorySupported) {
        // OK.
    } else {
        isFreeTrial_ = Result::Unsupported;
        subscriptionExpireDate_ = std::numeric_limits<int>::max();
        remainedTime_ = std::numeric_limits<int>::max();
        isIntroductoryPricePeriod_ = Result::Unsupported;
    }

    // iOS.
    subscriptionCancelDate_ = 0;
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
