//
//  StoreIStoreCallback.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_STORE_CALLBACK_HPP
#define EE_X_STORE_I_STORE_CALLBACK_HPP

#ifdef __cplusplus

#include <string>
#include <vector>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IStoreCallback {
public:
    virtual ~IStoreCallback() = default;

    virtual void onSetupFailed(InitializationFailureReason reason) = 0;
    virtual void
    onProductsRetrieved(const std::vector<ProductDescription>& products) = 0;
    virtual void onPurchaseSucceeded(const std::string& storeSpecificId,
                                     const std::string& receipt,
                                     const std::string& transactionId) = 0;
    virtual void
    onPurchaseFailed(const PurchaseFailureDescription& description) = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_STORE_CALLBACK_HPP */
