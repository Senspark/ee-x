//
//  StoreITransactionHistoryExtensions.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_I_TRANSACTION_HISTORY_EXTENSIONS_HPP
#define EE_X_STORE_I_TRANSACTION_HISTORY_EXTENSIONS_HPP

#include "ee/store/private/StoreIStoreExtension.hpp"

namespace ee {
namespace store {
class ITransactionHistoryExtensions : public virtual IStoreExtension {
public:
    virtual PurchaseFailureDescription
    getLastPurchaseFailureDescription() const = 0;
    virtual StoreSpecificPurchaseErrorCode
    getLastStoreSpecificPurchaseErrorCode() const = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_TRANSACTION_HISTORY_EXTENSIONS_HPP */
