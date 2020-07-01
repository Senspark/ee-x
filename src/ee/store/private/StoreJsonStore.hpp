//
//  StoreJsonStore.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_JSON_STORE_HPP
#define EE_X_STORE_JSON_STORE_HPP

#include "StoreIStoreConfiguration.hpp"
#include "StoreIStoreExtension.hpp"
#include "ee/store/StoreIStore.hpp"
#include "ee/store/private/StoreIUnityCallback.hpp"

namespace ee {
namespace store {
class JsonStore : public IStore,
                  public IUnityCallback,
                  public IStoreExtension,
                  public IStoreConfiguration {
public:
    JsonStore();

    bool storeTestEnabled() const;
    void storeTestEnabled(bool enabled);

    void setNativeStore(const std::shared_ptr<INativeStore>& callback);

    virtual void
    initialize(const std::shared_ptr<IStoreCallback>& callback) override;
    virtual void retrieveProducts(
        const std::vector<std::shared_ptr<ProductDefinition>>& products)
        override;
    virtual void
    purchase(const std::shared_ptr<ProductDefinition>& product) override;
    virtual void
    finishTransaction(const std::shared_ptr<ProductDefinition>& product,
                      const std::string& transactionId) override;

    virtual void onSetupFailed(const std::string& json) override;
    virtual void onProductsRetrieved(const std::string& json) override;
    virtual void onPurchaseSucceeded(const std::string& id,
                                     const std::string& receipt,
                                     const std::string& transactionId) override;
    virtual void onPurchaseFailed(const std::string& json) override;

    const PurchaseFailureDescription& getLastPurchaseFailureDescription() const;

protected:
    std::shared_ptr<IStoreCallback> unity_;

private:
    void onPurchaseFailed(const PurchaseFailureDescription& failure,
                          const std::string& json = "");

    bool isManagedStoreEnabled_;
    bool isRefreshing_;
    bool isFirstTimeRetrievingProducts_;

    StoreSpecificPurchaseErrorCode lastPurchaseErrorCode_;
    std::shared_ptr<INativeStore> store_;
    std::shared_ptr<PurchaseFailureDescription> lastPurchaseFailureDescription_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_JSON_STORE_HPP */
