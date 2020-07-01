//
//  StoreJsonStore.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/private/StoreJsonStore.hpp"

#include "ee/store/StoreIStoreCallback.hpp"
#include "ee/store/StoreInitializationFailureReason.hpp"
#include "ee/store/StoreProductCollection.hpp"
#include "ee/store/StorePurchaseFailureDescription.hpp"
#include "ee/store/private/StoreINativeStore.hpp"
#include "ee/store/private/StoreJsonSerializer.hpp"
#include "ee/store/private/StoreStoreSpecificPurchaseErrorCode.hpp"

namespace ee {
namespace store {
using Self = JsonStore;

Self::JsonStore() {
    isManagedStoreEnabled_ = true;
    isRefreshing_ = false;
    isFirstTimeRetrievingProducts_ = true;
    lastPurchaseErrorCode_ = StoreSpecificPurchaseErrorCode::Unknown;
}

void Self::initialize(const std::shared_ptr<IStoreCallback>& callback) {
    unity_ = callback;
}

void Self::retrieveProducts(
    const std::vector<std::shared_ptr<ProductDefinition>>& products) {
    store_->retrieveProducts(
        JsonSerializer::serializeProductDefinitions(products));
    isFirstTimeRetrievingProducts_ = false;
}

void Self::purchase(const std::shared_ptr<ProductDefinition>& product) {
    store_->purchase(JsonSerializer::serializeProductDefinition(product));
}

void Self::finishTransaction(const std::shared_ptr<ProductDefinition>& product,
                             const std::string& transactionId) {
    store_->finishTransaction(
        product == nullptr
            ? ""
            : JsonSerializer::serializeProductDefinition(product),
        transactionId);
}

void Self::onSetupFailed(const std::string& json) {
    // FIXME.
    assert(false);
    unity_->onSetupFailed(InitializationFailureReason::NoProductsAvailable);
}

void Self::onProductsRetrieved(const std::string& json) {
    unity_->onProductsRetrieved(
        JsonSerializer::deserializeProductDescriptions(json));
}

void Self::onPurchaseSucceeded(const std::string& id,
                               const std::string& receipt,
                               const std::string& transactionId) {
    unity_->onPurchaseSucceeded(id, receipt, transactionId);
}

void Self::onPurchaseFailed(const std::string& json) {
    onPurchaseFailed(JsonSerializer::deserializeFailureReason(json), json);
}

void Self::onPurchaseFailed(const PurchaseFailureDescription& failure,
                            const std::string& json) {
    lastPurchaseFailureDescription_ =
        std::make_shared<PurchaseFailureDescription>(failure);
    // FIXME.
    //  lastPurchaseErrorCode_ =
    unity_->onPurchaseFailed(failure);
}

const PurchaseFailureDescription&
Self::getLastPurchaseFailureDescription() const {
    return *lastPurchaseFailureDescription_;
}
} // namespace store
} // namespace ee
