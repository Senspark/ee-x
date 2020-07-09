//
//  StoreAndroidJavaStore.hpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#ifndef EE_X_STORE_ANDROID_JAVA_STORE_HPP
#define EE_X_STORE_ANDROID_JAVA_STORE_HPP

#include "ee/store/StoreIGooglePlayConfiguration.hpp"
#include "ee/store/StoreIGooglePlayStoreExtensions.hpp"
#include "ee/store/private/StoreINativeStore.hpp"
#include "ee/store/private/StoreIUnityCallback.hpp"

namespace ee {
namespace store {
class AndroidJavaStore : public INativeStore,
                         public IUnityCallback,
                         public IGooglePlayStoreExtensions,
                         public virtual IStoreExtension,
                         public IGooglePlayConfiguration,
                         public virtual IStoreConfiguration {
public:
    explicit AndroidJavaStore(IMessageBridge& bridge,
                              const std::shared_ptr<IUnityCallback>& callback);

    virtual ~AndroidJavaStore() override;

    virtual void retrieveProducts(const std::string& json) override;
    virtual void purchase(const std::string& productJson) override;
    virtual void finishTransaction(const std::string& productJson,
                                   const std::string& transactionId) override;

    virtual void onSetupFailed(const std::string& json) override;
    virtual void onProductsRetrieved(const std::string& json) override;
    virtual void onPurchaseSucceeded(const std::string& id,
                                     const std::string& receipt,
                                     const std::string& transactionId) override;
    virtual void onPurchaseFailed(const std::string& json) override;

    virtual void setPublicKey(const std::string& key) override;
    virtual std::map<std::string, std::string>
    getProductJsonDictionary() const override;
    virtual void
    restoreTransactions(const std::function<void(bool)>& callback) override;
    virtual void
    finishAdditionalTransaction(const std::string& productId,
                                const std::string& transactionId) override;

private:
    IMessageBridge& bridge_;
    std::shared_ptr<IUnityCallback> forwardTo_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_ANDROID_JAVA_STORE_HPP */
