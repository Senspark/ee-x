//
//  StoreIInternalStoreListener.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_I_INTERNAL_STORE_LISTENER_HPP
#define EE_X_STORE_I_INTERNAL_STORE_LISTENER_HPP

#include <memory>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IInternalStoreListener {
public:
    virtual ~IInternalStoreListener() = default;

    virtual void onInitializeFailed(InitializationFailureReason reason) = 0;
    virtual PurchaseProcessingResult
    processPurchase(const std::shared_ptr<Product>& product) = 0;
    virtual void onPurchaseFailed(const std::shared_ptr<Product>& product,
                                  PurchaseFailureReason reason) = 0;
    virtual void
    onInitialized(const std::shared_ptr<IStoreController>& controller) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_INTERNAL_STORE_LISTENER_HPP */
