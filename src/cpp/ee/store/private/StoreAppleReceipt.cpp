//
//  StoreAppleReceipt.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/private/StoreAppleReceipt.hpp"

namespace ee {
namespace store {
using Self = AppleReceipt;

Self::AppleReceipt(
    const std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>>&
        inAppPurchaseReceipts,
    const std::string& bundleId,                   //
    const std::string& appVersion,                 //
    int expirationDate,                            //
    const std::string& originalApplicationVersion, //
    int receiptCreationDate)
    : inAppPurchaseReceipts_(inAppPurchaseReceipts)
    , bundleId_(bundleId)
    , appVersion_(appVersion)
    , expirationDate_(expirationDate)
    , originalApplicationVersion_(originalApplicationVersion)
    , receiptCreationDate_(receiptCreationDate) {}

const std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>>&
Self::inAppPurchaseReceipts() const {
    return inAppPurchaseReceipts_;
}

const std::string& Self::bundleId() const {
    return bundleId_;
}

const std::string& Self::appVersion() const {
    return appVersion_;
}

int Self::expirationDate() const {
    return expirationDate_;
}

const std::string& Self::originalApplicationVersion() const {
    return originalApplicationVersion_;
}

int Self::receiptCreationDate() const {
    return receiptCreationDate_;
}
} // namespace store
} // namespace ee
