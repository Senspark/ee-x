//
//  StoreAppleStoreImpl.hpp
//  Pods
//
//  Created by eps on 6/29/20.
//

#ifndef EE_X_STORE_APPLE_STORE_IMPL_HPP
#define EE_X_STORE_APPLE_STORE_IMPL_HPP

#include "ee/store/private/StoreIAppleConfiguration.hpp"
#include "ee/store/private/StoreIAppleExtensions.hpp"
#include "ee/store/private/StoreJsonStore.hpp"

namespace ee {
namespace store {
class AppleStoreImpl : public JsonStore,
                       public IAppleExtensions,
                       public virtual IStoreExtension,
                       public IAppleConfiguration,
                       public virtual IStoreConfiguration {
public:
    void setNativeStore(const std::shared_ptr<INativeAppleStore>& apple);

    virtual std::string appReceipt() const override;
    virtual bool canMakePayments() const override;
    virtual bool simulateAskToBuy() const override;
    virtual void simulateAskToBuy(bool value) override;
    virtual std::string getTransactionReceiptForProduct(
        const std::shared_ptr<Product>& product) override;
    virtual void onProductsRetrieved(const std::string& json) override;
    virtual void
    restoreTransactions(const std::function<void(bool)>& callback) override;
    virtual void refreshAppReceipt(
        const std::function<void(const std::string& receipt)>& successCallback,
        const std::function<void()>& errorCallback) override;
    virtual void registerPurchaseDeferredListener(
        const std::function<void(const std::shared_ptr<Product>& product)>&
            callback) override;
    virtual std::map<std::string, std::string>
    getIntroductoryPriceDictionary() const override;
    virtual std::map<std::string, std::string>
    getProductDetails() const override;

private:
    void onPurchaseDeferred(const std::string& productId);
    void onTransactionsRestoredSuccess();
    void onTransactionsRestoredFail(const std::string& error);
    void onAppReceiptRetrieved(const std::string& receipt);
    void onAppReceiptRefreshedFailed();
    void processMessage(const std::string& subject, const std::string& payload,
                        const std::string& receipt,
                        const std::string& transactionId);

    virtual void onPurchaseSucceeded(const std::string& id,
                                     const std::string& receipt,
                                     const std::string& transactionId) override;

    std::shared_ptr<AppleReceipt>
    getAppleReceiptFromBase64String(const std::string& receipt) const;
    bool isValidPurchaseState(const std::shared_ptr<AppleReceipt>& receipt,
                              const std::string& id);

    std::function<void(const std::shared_ptr<Product>& product)>
        deferredCallback_;
    std::function<void()> refreshReceiptError_;
    std::function<void(const std::string& receipt)> refreshReceiptSuccess_;
    std::function<void(bool)> restoreCallback_;
    std::shared_ptr<INativeAppleStore> native_;
    std::string productsJson_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_APPLE_STORE_IMPL_HPP */
