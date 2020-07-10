//
//  StoreStoreListenerProxy.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/private/StoreStoreListenerProxy.hpp"

#include "ee/store/private/StoreIStoreListener.hpp"

namespace ee {
namespace store {
using Self = StoreListenerProxy;

Self::StoreListenerProxy(const std::shared_ptr<IStoreListener>& forwardTo,
                         const std::shared_ptr<IExtensionProvider>& extensions)
    : forwardTo_(forwardTo)
    , extensions_(extensions) {}

void Self::onInitialized(const std::shared_ptr<IStoreController>& controller) {
    forwardTo_->onInitialized(controller, extensions_);
}

void Self::onInitializeFailed(InitializationFailureReason reason) {
    forwardTo_->onInitializeFailed(reason);
}

PurchaseProcessingResult
Self::processPurchase(const std::shared_ptr<Product>& product) {
    return forwardTo_->processPurchase(product);
}

void Self::onPurchaseFailed(const std::shared_ptr<Product>& product,
                            PurchaseFailureReason reason) {
    forwardTo_->onPurchaseFailed(product, reason);
}
} // namespace store
} // namespace ee
