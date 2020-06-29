//
//  StoreAppleStoreImpl.cpp
//  Pods
//
//  Created by eps on 6/29/20.
//

#include "StoreAppleStoreImpl.hpp"

#include "StoreINativeAppleStore.hpp"
#include "StoreProduct.hpp"
#include "StoreProductDefinition.hpp"

namespace ee {
namespace store {
using Self = AppleStoreImpl;

void Self::setNativeStore(const std::shared_ptr<INativeAppleStore>& apple) {
    native_ = apple;
    apple->setPurchasingCallback(
        [](const std::string& subject, const std::string& payload,
           const std::string& receipt, const std::string& transactionId) {
            //
        });
}

std::string Self::appReceipt() const {
    return native_->appReceipt();
}

bool Self::canMakePayments() const {
    return native_->canMakePayments();
}

bool Self::simulateAskToBuy() const {
    return native_->simulateAskToBuy();
}

void Self::simulateAskToBuy(bool value) {
    native_->simulateAskToBuy(value);
}

std::string
Self::getTransactionReceiptForProduct(const std::shared_ptr<Product>& product) {
    return native_->getTransactionReceiptForProductId(
        product->definition()->storeSpecificId());
}

void Self::restoreTransactions(const std::function<void(bool)>& callback) {
    restoreCallback_ = callback;
    native_->restoreTransactions();
}

void Self::refreshAppReceipt(
    const std::function<void(const std::string&)>& successCallback,
    const std::function<void()>& errorCallback) {
    refreshReceiptSuccess_ = successCallback;
    refreshReceiptError_ = errorCallback;
    native_->refreshAppReceipt();
}

void Self::registerPurchaseDeferredListener(
    const std::function<void(const std::shared_ptr<Product>& product)>&
        callback) {
    deferredCallback_ = callback;
}
} // namespace store
} // namespace ee
