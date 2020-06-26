//
//  StoreINativeAppleStore.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_NATIVE_APP_STORE_HPP
#define EE_X_STORE_I_NATIVE_APP_STORE_HPP

#include "StoreINativeStore.hpp"

namespace ee {
namespace store {
class INativeAppleStore : public INativeStore {
public:
    virtual void restoreTransactions() = 0;
    virtual void refreshAppReceipt() = 0;
    virtual void addTransactionObserver() = 0;
    virtual void
    setApplicationUsername(const std::string& applicationUsername) = 0;
    virtual std::string appReceipt() const = 0;
    virtual bool canMakePayments() const = 0;
    virtual bool simulateAskToBuy() const = 0;
    virtual void simulateAskToBuy(bool enabled) = 0;
    virtual std::string
    getTransactionReceiptForProductId(const std::string& productId) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_NATIVE_APP_STORE_HPP */
