//
//  StoreIosStoreBindings.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_IOS_STORE_BINDINGS_HPP
#define EE_X_STORE_IOS_STORE_BINDINGS_HPP

#include "StoreINativeAppleStore.hpp"

namespace ee {
namespace store {
class IosStoreBindings : public INativeAppleStore {
public:
    IosStoreBindings();

    virtual std::string appReceipt() const override;
    virtual bool canMakePayments() const override;
    virtual bool simulateAskToBuy() const override;
    virtual void simulateAskToBuy(bool enabled) override;

    virtual void retrieveProducts(const std::string& json) override;
    virtual void purchase(const std::string& productJson) override;
    virtual void finishTransaction(const std::string& productJson,
                                   const std::string& transactionId) override;

    virtual void restoreTransactions() override;
    virtual void refreshAppReceipt() override;
    virtual void addTransactionObserver() override;
    virtual void
    setApplicationUsername(const std::string& applicationUsername) override;

    virtual std::string
    getTransactionReceiptForProductId(const std::string& productId) override;

private:
    IMessageBridge& bridge_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_IOS_STORE_BINDINGS_HPP */
