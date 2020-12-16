//
//  StoreAppleReceipt.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_APPLE_RECEIPT_HPP
#define EE_X_STORE_APPLE_RECEIPT_HPP

#include <string>
#include <vector>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class AppleReceipt {
public:
    explicit AppleReceipt(
        const std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>>&
            inAppPurchaseReceipts,
        const std::string& bundleId,                   //
        const std::string& appVersion,                 //
        int expirationDate,                            //
        const std::string& originalApplicationVersion, //
        int receiptCreationDate);

    const std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>>&
    inAppPurchaseReceipts() const;
    const std::string& bundleId() const;
    const std::string& appVersion() const;
    int expirationDate() const;
    const std::string& originalApplicationVersion() const;
    int receiptCreationDate() const;

private:
    std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>>
        inAppPurchaseReceipts_;
    std::string bundleId_;
    std::string appVersion_;
    int expirationDate_;
    std::string originalApplicationVersion_;
    int receiptCreationDate_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_APPLE_RECEIPT_HPP */
