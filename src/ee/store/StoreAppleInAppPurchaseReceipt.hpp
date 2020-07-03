//
//  StoreAppleInAppPurchaseReceipt.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_APPLE_IN_APP_PURCHASE_RECEIPT_HPP
#define EE_X_STORE_APPLE_IN_APP_PURCHASE_RECEIPT_HPP

#ifdef __cplusplus

#include "ee/store/StoreIPurchaseReceipt.hpp"

namespace ee {
namespace store {
class AppleInAppPurchaseReceipt : public IPurchaseReceipt {
public:
    explicit AppleInAppPurchaseReceipt(                   //
        int quantity,                                     //
        const std::string& productId,                     //
        const std::string& transactionId,                 //
        const std::string& originalTransactionIdentifier, //
        int purchaseDate,                                 //
        int originalPurchaseDate,                         //
        int subscriptionExpirationDate,                   //
        int cancellationDate,                             //
        bool isFreeTrial,                                 //
        AppleStoreProductType productType,                //
        bool isIntroductoryPricePeriod);

    int quantity() const;
    virtual std::string productId() const override;
    virtual std::string transactionId() const override;
    const std::string& originalTransactionIdentifier() const;
    virtual int purchaseDate() const override;
    int originalPurchaseDate() const;
    int subscriptionExpirationDate() const;
    int cancellationDate() const;
    bool isFreeTrial() const;
    AppleStoreProductType productType() const;
    bool isIntroductoryPricePeriod() const;

private:
    int quantity_;
    std::string productId_;
    std::string transactionId_;
    std::string originalTransactionIdentifier_;
    int purchaseDate_;
    int originalPurchaseDate_;
    int subscriptionExpirationDate_;
    int cancellationDate_;
    bool isFreeTrial_;
    AppleStoreProductType productType_;
    bool isIntroductoryPricePeriod_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_APPLE_IN_APP_PURCHASE_RECEIPT_HPP */
