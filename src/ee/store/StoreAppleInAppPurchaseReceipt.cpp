//
//  StoreAppleInAppPurchaseReceipt.cpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#include "ee/store/StoreAppleInAppPurchaseReceipt.hpp"

namespace ee {
namespace store {
using Self = AppleInAppPurchaseReceipt;

Self::AppleInAppPurchaseReceipt(                      //
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
    bool isIntroductoryPricePeriod)
    : quantity_(quantity)
    , productId_(productId)
    , transactionId_(transactionId)
    , originalTransactionIdentifier_(originalTransactionIdentifier)
    , purchaseDate_(purchaseDate)
    , originalPurchaseDate_(originalPurchaseDate)
    , subscriptionExpirationDate_(subscriptionExpirationDate)
    , cancellationDate_(cancellationDate)
    , isFreeTrial_(isFreeTrial)
    , productType_(productType)
    , isIntroductoryPricePeriod_(isIntroductoryPricePeriod) {}

int Self::quantity() const {
    return quantity_;
}

std::string Self::productId() const {
    return productId_;
}

std::string Self::transactionId() const {
    return transactionId_;
}

const std::string& Self::originalTransactionIdentifier() const {
    return originalTransactionIdentifier_;
}

int Self::purchaseDate() const {
    return purchaseDate_;
}

int Self::originalPurchaseDate() const {
    return originalPurchaseDate_;
}

int Self::subscriptionExpirationDate() const {
    return subscriptionExpirationDate_;
}

int Self::cancellationDate() const {
    return cancellationDate_;
}

bool Self::isFreeTrial() const {
    return isFreeTrial_;
}

AppleStoreProductType Self::productType() const {
    return productType_;
}

bool Self::isIntroductoryPricePeriod() const {
    return isIntroductoryPricePeriod_;
}
} // namespace store
} // namespace ee
