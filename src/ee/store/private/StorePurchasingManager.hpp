//
//  StorePurchasingManager.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PURCHASING_MANAGER_HPP
#define EE_X_STORE_PURCHASING_MANAGER_HPP

#include <set>

#include "ee/store/private/StoreIStoreCallback.hpp"
#include "ee/store/private/StoreIStoreController.hpp"

namespace ee {
namespace store {
class PurchasingManager
    : public std::enable_shared_from_this<PurchasingManager>,
      public IStoreCallback,
      public IStoreController {
public:
    explicit PurchasingManager(
        const std::shared_ptr<ITransactionLog>& transactionLog, //
        const Logger& logger,                                   //
        const std::shared_ptr<IStore>& store,                   //
        const std::string& storeName);

    virtual bool useTransactionLog() const override;
    virtual void useTransactionLog(bool enabled) override;

    virtual void
    initiatePurchase(const std::shared_ptr<Product>& product) override;
    virtual void initiatePurchase(const std::string& itemId) override;
    virtual void
    confirmPendingPurchase(const std::shared_ptr<Product>& product) override;

    virtual std::shared_ptr<ProductCollection> products() const override;

    void
    initialize(const std::shared_ptr<IInternalStoreListener>& listener,
               const std::set<std::shared_ptr<ProductDefinition>>& products);

private:
    virtual void onPurchaseSucceeded(const std::string& storeSpecificId,
                                     const std::string& receipt,
                                     const std::string& transactionId) override;
    virtual void onSetupFailed(InitializationFailureReason reason) override;
    virtual void
    onPurchaseFailed(const PurchaseFailureDescription& description) override;
    virtual void onProductsRetrieved(
        const std::vector<std::shared_ptr<ProductDescription>>& products)
        override;

    void processPurchaseIfNew(const std::shared_ptr<Product>& product);
    void checkForInitialization();
    std::string formatUnifiedReceipt(const std::string& platformReceipt,
                                     const std::string& transactionId);

    std::shared_ptr<ITransactionLog> transactionLog_;
    const Logger& logger_;
    std::shared_ptr<IStore> store_;
    std::string storeName_;
    bool useTransactionLog_;
    bool initialized_;
    std::shared_ptr<IInternalStoreListener> listener_;
    std::shared_ptr<ProductCollection> products_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASING_MANAGER_HPP */
