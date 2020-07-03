//
//  StoreSubscriptionInfo.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_SUBSCRIPTION_INFO_HPP
#define EE_X_STORE_SUBSCRIPTION_INFO_HPP

#ifdef __cplusplus

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class SubscriptionInfo {
public:
    explicit SubscriptionInfo(
        const std::shared_ptr<AppleInAppPurchaseReceipt>& receipt,
        const std::string& introJson);

    const std::string& getProductId() const;
    int getPurchaseDate() const;
    Result isSubscribed() const;
    Result isExpired() const;
    Result isCancelled() const;
    Result isFreeTrial() const;
    Result isAutoRenewing() const;
    int getRemainingTime() const;
    Result isIntroductoryPricePeriod() const;
    int getIntroductoryPricePeriod() const;
    const std::string& getIntroductoryPrice() const;
    int getIntroductoryPricePeriodCycles() const;
    int getExpireDate() const;
    int getCancelDate() const;
    int getFreeTrialPeriod() const;
    int getSubscriptionPeriod() const;
    const std::string& getFreeTrialPeriodString() const;
    const std::string& getSkuDetails() const;

private:
    const Logger& logger_;
    Result isSubscribed_;
    Result isExpired_;
    Result isCancelled_;
    Result isFreeTrial_;
    Result isAutoRenewing_;
    Result isIntroductoryPricePeriod_;
    std::string productId_;
    int purchaseDate_;
    int subscriptionExpireDate_;
    int subscriptionCancelDate_;
    int remainedTime_;
    std::string introductoryPrice_;
    int introductoryPricePeriod_;
    int introductoryPriceCycles_;
    int freeTrialPeriod_;
    int subscriptionPeriod_;
    std::string freeTrialPeriodString_;
    std::string skuDetails_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_SUBSCRIPTION_INFO_HPP */
