//
//  StoreAppleStoreImpl.hpp
//  Pods
//
//  Created by eps on 6/29/20.
//

#ifndef EE_X_STORE_APPLE_STORE_IMPL_HPP
#define EE_X_STORE_APPLE_STORE_IMPL_HPP

#include "StoreIAppleConfiguration.hpp"
#include "StoreIAppleExtensions.hpp"

namespace ee {
namespace store {
class AppleStoreImpl : public IAppleExtensions, IAppleConfiguration {
public:
    void setNativeStore(const std::shared_ptr<INativeAppleStore>& apple);

    virtual std::string appReceipt() const override;
    virtual bool canMakePayments() const override;
    virtual bool simulateAskToBuy() const override;
    virtual void simulateAskToBuy(bool value) override;
    virtual std::string getTransactionReceiptForProduct(
        const std::shared_ptr<Product>& product) override;

    virtual void
    restoreTransactions(const std::function<void(bool)>& callback) override;
    virtual void refreshAppReceipt(
        const std::function<void(const std::string&)>& successCallback,
        const std::function<void()>& errorCallback) override;
    virtual void registerPurchaseDeferredListener(
        const std::function<void(const std::shared_ptr<Product>& product)>&
            callback) override;

private:
    std::function<void(const std::shared_ptr<Product>& product)>
        deferredCallback_;
    std::function<void()> refreshReceiptError_;
    std::function<void(const std::string&)> refreshReceiptSuccess_;
    std::function<void(bool)> restoreCallback_;
    std::shared_ptr<INativeAppleStore> native_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_APPLE_STORE_IMPL_HPP */
