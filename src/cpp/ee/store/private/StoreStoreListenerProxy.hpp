//
//  StoreStoreListenerProxy.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_STORE_LISTENER_PROXY_HPP
#define EE_X_STORE_STORE_LISTENER_PROXY_HPP

#include "ee/store/private/StoreIInternalStoreListener.hpp"

namespace ee {
namespace store {
class StoreListenerProxy : public IInternalStoreListener {
public:
    explicit StoreListenerProxy(
        const std::shared_ptr<IStoreListener>& forwardTo,
        const std::shared_ptr<IExtensionProvider>& extensions,
        const std::shared_ptr<ILibraryAnalytics>& analytics);

    virtual void
    onInitialized(const std::shared_ptr<IStoreController>& controller) override;
    virtual void
    onInitializeFailed(InitializationFailureReason reason) override;
    virtual PurchaseProcessingResult
    processPurchase(const std::shared_ptr<Product>& product) override;
    virtual void onPurchaseFailed(const std::shared_ptr<Product>& product,
                                  PurchaseFailureReason reason) override;

private:
    std::shared_ptr<IStoreListener> forwardTo_;
    std::shared_ptr<IExtensionProvider> extensions_;
    std::shared_ptr<ILibraryAnalytics> analytics_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_STORE_LISTENER_PROXY_HPP */
