//
//  StorePurchasingManager.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PURCHASING_MANAGER_HPP
#define EE_X_STORE_PURCHASING_MANAGER_HPP

#ifdef __cplusplus

#include "ee/store/StoreIStoreCallback.hpp"
#include "ee/store/StoreIStoreController.hpp"

namespace ee {
namespace store {
class PurchasingManager : public IStoreCallback, public IStoreController {
public:
    explicit PurchasingManager(
        const std::shared_ptr<ITransactionLog>& transactionLog, //
        const Logger& logger,                                   //
        const std::shared_ptr<IStore>& store,                   //
        const std::string& storeName);

    bool useTransactionLog() const;
    void useTransactionLog(bool enabled);

    virtual void
    initiatePurchase(const std::shared_ptr<Product>& product) override;
    virtual void initiatePurchase(const std::string& productId) override;
    virtual void
    confirmPendingPurchase(const std::shared_ptr<Product>& product) override;

    virtual std::shared_ptr<ProductCollection> getProducts() const override;

    void
    initialize(const std::shared_ptr<IStoreListener>& listener,
               const std::vector<std::shared_ptr<ProductDefinition>>& products);

private:
    virtual void onPurchaseSucceeded(const std::string& storeSpecificId,
                                     const std::string& receipt,
                                     const std::string& transactionId) override;
    virtual void onSetupFailed(InitializationFailureReason reason) override;
    virtual void
    onPurchaseFailed(const PurchaseFailureDescription& description) override;
    virtual void onProductsRetrieved(
        const std::vector<ProductDescription>& products) override;

    void processPurchaseIfNew(const std::shared_ptr<Product>& product);
    void checkForInitialization();
    std::string FormatUnifiedReceipt(const std::string& platformReceipt,
                                     const std::string& transactionId);

    std::shared_ptr<ITransactionLog> transactionLog_;
    const Logger& logger_;
    std::shared_ptr<IStore> store_;
    std::string storeName_;
    bool useTransactionLog_;
    bool initialized_;
    std::shared_ptr<IStoreListener> listener_;
    std::shared_ptr<ProductCollection> products_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PURCHASING_MANAGER_HPP */
