//
//  StoreITransactionHistoryExtensions.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_I_TRANSACTION_HISTORY_EXTENSIONS_HPP
#define EE_X_STORE_I_TRANSACTION_HISTORY_EXTENSIONS_HPP

#ifdef __cplusplus

#include "ee/store/StoreIStoreExtension.hpp"

namespace ee {
namespace store {
class ITransactionHistoryExtensions : public virtual IStoreExtension {
public:
    virtual PurchaseFailureDescription
    getLastPurchaseFailureDescription() const = 0;
    virtual StoreSpecificPurchaseErrorCode
    getLastStoreSPecificPurchaseErrorCode() const = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_TRANSACTION_HISTORY_EXTENSIONS_HPP */
