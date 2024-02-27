//
//  StoreIosStoreBindings.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_IOS_STORE_BINDINGS_HPP
#define EE_X_STORE_IOS_STORE_BINDINGS_HPP

#include "ee/store/private/StoreINativeAppleStore.hpp"

namespace ee {
namespace store {
class IosStoreBindings : public INativeAppleStore {
public:
    explicit IosStoreBindings(IMessageBridge& bridge);
    virtual ~IosStoreBindings() override;

    virtual void
    setPurchasingCallback(const PurchasingCallback& callback) override;
    virtual std::string appReceipt() const override;
    virtual bool canMakePayments() const override;
    virtual bool simulateAskToBuy() const override;
    virtual void simulateAskToBuy(bool enabled) override;
    virtual void retrieveProducts(const std::string& json) override;
    virtual void purchase(const std::string& productJson) override;
    virtual void finishTransaction(const std::string& productJson,
                                   const std::string& transactionId) override;
    virtual void setObfuscationAccountId(const std::string& obfuscationAccountId) override {}
    virtual void restoreTransactions() override;
    virtual void refreshAppReceipt() override;
    virtual void addTransactionObserver() override;

private:
    IMessageBridge& bridge_;
    PurchasingCallback purchasingCallback_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_IOS_STORE_BINDINGS_HPP */
