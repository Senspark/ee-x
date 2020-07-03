//
//  StoreAppleStoreImpl.cpp
//  Pods
//
//  Created by eps on 6/29/20.
//

#include "ee/store/private/StoreAppleStoreImpl.hpp"

#include <ee/core/Logger.hpp>

#include "ee/store/StoreAppleInAppPurchaseReceipt.hpp"
#include "ee/store/StoreAppleReceipt.hpp"
#include "ee/store/StoreAppleStoreProductType.hpp"
#include "ee/store/StoreIStoreCallback.hpp"
#include "ee/store/StoreProduct.hpp"
#include "ee/store/StoreProductCollection.hpp"
#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/StoreProductDescription.hpp"
#include "ee/store/StoreResult.hpp"
#include "ee/store/StoreSubscriptionInfo.hpp"
#include "ee/store/private/StoreAppleReceiptParser.hpp"
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
    std::optional<std::vector<std::shared_ptr<ProductDescription>>>
        descriptionList2;
    productsJson_ = json;
    do {
        if (native_ == nullptr) {
            break;
        }
        auto appReceipt = native_->appReceipt();
        if (appReceipt.empty()) {
            break;
        }
        descriptionList2 = std::vector<std::shared_ptr<ProductDescription>>();
        auto receipt = getAppleReceiptFromBase64String(appReceipt);
        if (receipt == nullptr || receipt->inAppPurchaseReceipts().empty()) {
            break;
        }
        for (auto&& description : descriptionList1) {
            std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>> all;
            for (auto&& r : receipt->inAppPurchaseReceipts()) {
                if (r->productId() == description->storeSpecificId()) {
                    all.push_back(r);
                }
            }
            if (all.empty()) {
                descriptionList2->push_back(description);
                continue;
            }
            std::stable_sort(
                all.begin(), all.end(),
                [](const std::shared_ptr<AppleInAppPurchaseReceipt>& b,
                   const std::shared_ptr<AppleInAppPurchaseReceipt>& a) {
                    return a->purchaseDate() < b->purchaseDate();
                });
            auto&& r = all[0];
            switch (r->productType()) {
            case AppleStoreProductType::Consumable:
                descriptionList2->push_back(description);
                break;
            case AppleStoreProductType::AutoRenewingSubscription:
                if (SubscriptionInfo(r, "").isExpired() == Result::True) {
                    descriptionList2->push_back(description);
                    break;
                }
                [[fallthrough]];
            default:
                descriptionList2->push_back(
                    std::make_shared<ProductDescription>(
                        description->storeSpecificId(), description->metadata(),
                        appReceipt, r->transactionId()));
                break;
            }
        }
        for (auto&& appPurchaseReceipt : receipt->inAppPurchaseReceipts()) {
            logger_.debug("productId: %s",
                          appPurchaseReceipt->productId().c_str());
            logger_.debug("transactionId: %s",
                          appPurchaseReceipt->transactionId().c_str());
            logger_.debug(
                "originalTransactionIdentifier: %s",
                appPurchaseReceipt->originalTransactionIdentifier().c_str());
            logger_.debug("purchaseDate: %d",
                          appPurchaseReceipt->purchaseDate());
            logger_.debug("originalPurchaseDate: %d",
                          appPurchaseReceipt->originalPurchaseDate());
            logger_.debug("subscriptionExpirationDate: %d",
                          appPurchaseReceipt->subscriptionExpirationDate());
        }
    } while (false);
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
    return AppleReceiptParser().parse(receipt);
}

bool Self::isValidPurchaseState(const std::shared_ptr<AppleReceipt>& receipt,
                                const std::string& id) {
    if (receipt == nullptr) {
        return true;
    }
    if (receipt->inAppPurchaseReceipts().empty()) {
        return true;
    }
    std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>> all;
    for (auto&& r : receipt->inAppPurchaseReceipts()) {
        if (r->productId() == id) {
            all.push_back(r);
        }
    }
    if (all.empty()) {
        return true;
    }
    std::stable_sort(all.begin(), all.end(),
                     [](const std::shared_ptr<AppleInAppPurchaseReceipt>& b,
                        const std::shared_ptr<AppleInAppPurchaseReceipt>& a) {
                         return a->purchaseDate() < b->purchaseDate();
                     });
    auto&& r = all[0];
    if (r->productType() == AppleStoreProductType::AutoRenewingSubscription &&
        SubscriptionInfo(r, "").isExpired() == Result::True) {
        return false;
    }
    return true;
}
} // namespace store
} // namespace ee
