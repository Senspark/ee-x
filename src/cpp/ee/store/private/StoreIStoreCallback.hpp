//
//  StoreIStoreCallback.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_STORE_CALLBACK_HPP
#define EE_X_STORE_I_STORE_CALLBACK_HPP

#include <string>
#include <vector>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IStoreCallback {
public:
    virtual ~IStoreCallback() = default;

    virtual std::shared_ptr<ProductCollection> products() const = 0;
    virtual void onSetupFailed(InitializationFailureReason reason) = 0;
    virtual void onProductsRetrieved(
        const std::vector<std::shared_ptr<ProductDescription>>& products) = 0;
    virtual void onPurchaseSucceeded(const std::string& storeSpecificId,
                                     const std::string& receipt,
                                     const std::string& transactionId) = 0;
    virtual void
    onPurchaseFailed(const PurchaseFailureDescription& description) = 0;
    virtual bool useTransactionLog() const = 0;
    virtual void useTransactionLog(bool value) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_STORE_CALLBACK_HPP */
