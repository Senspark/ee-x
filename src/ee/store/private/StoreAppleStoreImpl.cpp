//
//  StoreAppleStoreImpl.cpp
//  Pods
//
//  Created by eps on 6/29/20.
//

#include "ee/store/private/StoreAppleStoreImpl.hpp"

#include "ee/store/StoreIStoreCallback.hpp"
#include "ee/store/StoreProduct.hpp"
#include "ee/store/StoreProductCollection.hpp"
#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/private/StoreINativeAppleStore.hpp"
#include "ee/store/private/StoreJsonSerializer.hpp"

namespace ee {
namespace store {
using Self = AppleStoreImpl;

void Self::setNativeStore(const std::shared_ptr<INativeAppleStore>& apple) {
    JsonStore::setNativeStore(std::dynamic_pointer_cast<INativeStore>(apple));
    native_ = apple;
    apple->setPurchasingCallback(
        [this](const std::string& subject, const std::string& payload,
               const std::string& receipt, const std::string& transactionId) {
            processMessage(subject, payload, receipt, transactionId);
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

void Self::onProductsRetrieved(const std::string& json) {
    auto descriptionList1 =
        JsonSerializer::deserializeProductDescriptions(json);
    productsJson_ = json;
    if (native_) {
        auto appReceipt = native_->appReceipt();
        if (not appReceipt.empty()) {
            // FIXME: apple receipt verification.
        }
    }
    unity_->onProductsRetrieved(descriptionList1);
    native_->addTransactionObserver();
}

void Self::restoreTransactions(const std::function<void(bool)>& callback) {
    restoreCallback_ = callback;
    native_->restoreTransactions();
}

void Self::refreshAppReceipt(
    const std::function<void(const std::string& receipt)>& successCallback,
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

std::map<std::string, std::string>
Self::getIntroductoryPriceDictionary() const {
    return JsonSerializer::deserializeSubscriptionDescriptions(productsJson_);
}

std::map<std::string, std::string> Self::getProductDetails() const {
    return JsonSerializer::deserializeProductDetails(productsJson_);
}

void Self::onPurchaseDeferred(const std::string& productId) {
    if (deferredCallback_ == nullptr) {
        return;
    }
    auto product = unity_->products()->withStoreSpecificId(productId);
    if (product != nullptr) {
        deferredCallback_(product);
    }
}

void Self::onTransactionsRestoredSuccess() {
    if (restoreCallback_ == nullptr) {
        return;
    }
    restoreCallback_(true);
}

void Self::onTransactionsRestoredFail(const std::string& error) {
    if (restoreCallback_ == nullptr) {
        return;
    }
    restoreCallback_(false);
}

void Self::onAppReceiptRetrieved(const std::string& receipt) {
    if (receipt.empty() || refreshReceiptSuccess_ == nullptr) {
        return;
    }
    refreshReceiptSuccess_(receipt);
}

void Self::onAppReceiptRefreshedFailed() {
    if (refreshReceiptError_ == nullptr) {
        return;
    }
    refreshReceiptError_();
}

void Self::processMessage(const std::string& subject,
                          const std::string& payload,
                          const std::string& receipt,
                          const std::string& transactionId) {
    if (subject == "onTransactionsRestoredFail") {
        return onTransactionsRestoredFail(payload);
    }
    if (subject == "onAppReceiptRefreshFailed") {
        return onAppReceiptRefreshedFailed();
    }
    if (subject == "onProductPurchaseDeferred") {
        return onPurchaseDeferred(payload);
    }
    if (subject == "onTransactionsRestoredSuccess") {
        return onTransactionsRestoredSuccess();
    }
    if (subject == "OnPurchaseFailed") {
        return onPurchaseFailed(payload);
    }
    if (subject == "OnPurchaseSucceeded") {
        return onPurchaseSucceeded(payload, receipt, transactionId);
    }
    if (subject == "onAppReceiptRefreshed") {
        return onAppReceiptRetrieved(payload);
    }
    if (subject == "OnProductsRetrieved") {
        return onProductsRetrieved(payload);
    }
    if (subject == "OnSetupFailed") {
        return onSetupFailed(payload);
    }
}

void Self::onPurchaseSucceeded(const std::string& id,
                               const std::string& receipt,
                               const std::string& transactionId) {
    if (isValidPurchaseState(getAppleReceiptFromBase64String(receipt), id)) {
        JsonStore::onPurchaseSucceeded(id, receipt, transactionId);
    } else {
        finishTransaction(nullptr, transactionId);
    }
}

std::shared_ptr<AppleReceipt>
Self::getAppleReceiptFromBase64String(const std::string& receipt) const {
    // FIXME.
    return nullptr;
}

bool Self::isValidPurchaseState(
    const std::shared_ptr<AppleReceipt>& appleReceipt, const std::string& id) {
    // FIXME.
    return true;
}
} // namespace store
} // namespace ee
