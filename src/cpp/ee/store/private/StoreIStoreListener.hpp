//
//  StoreIStoreListener.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_STORE_LISTENER_HPP
#define EE_X_STORE_I_STORE_LISTENER_HPP

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IStoreListener {
public:
    virtual ~IStoreListener() = default;

    virtual void onInitializeFailed(InitializationFailureReason reason) = 0;
    virtual PurchaseProcessingResult
    processPurchase(const std::shared_ptr<Product>& product) = 0;
    virtual void onPurchaseFailed(const std::shared_ptr<Product>& product,
                                  PurchaseFailureReason reason) = 0;
    virtual void
    onInitialized(const std::shared_ptr<IStoreController>& controller,
                  const std::shared_ptr<IExtensionProvider>& extensions) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_STORE_LISTENER_HPP */
