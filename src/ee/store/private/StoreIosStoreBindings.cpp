//
//  StoreIosStoreBindings.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "StoreIosStoreBindings.hpp"

#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace store {
using Self = IosStoreBindings;

Self::IosStoreBindings()
    : bridge_(MessageBridge::getInstance()) {}

std::string Self::appReceipt() const {
    return bridge_.call("getUnityPurchasingAppReceipt");
}

bool Self::canMakePayments() const {
    return core::toBool(bridge_.call("getUnityPurchasingCanMakePayments"));
}

bool Self::simulateAskToBuy() const {
    return core::toBool(bridge_.call("getSimulateAskToBuy"));
}

void Self::simulateAskToBuy(bool enabled) {
    bridge_.call("setSimulateAskToBuy", core::toString(enabled));
}

void Self::retrieveProducts(const std::string& json) {
    bridge_.call("unityPurchasingRetrieveProducts", json);
}

void Self::purchase(const std::string& productJson) {
    bridge_.call("unityPurchasingPurchase", productJson);
}

void Self::finishTransaction(const std::string& productJson,
                             const std::string& transactionId) {
    nlohmann::json json;
    json["productJson"] = productJson;
    json["transactionId"] = transactionId;
    bridge_.call("unityPurchasingFinishTransaction", json.dump());
}

void Self::restoreTransactions() {
    bridge_.call("unityPurchasingRestoreTransactions");
}

void Self::refreshAppReceipt() {
    bridge_.call("unityPurchasingRefreshAppReceipt");
}

void Self::addTransactionObserver() {
    bridge_.call("unityPurchasingAddTransactionObserver");
}

void Self::setApplicationUsername(const std::string& applicationUsername) {
    bridge_.call("unityPurchasingSetApplicationUsername", applicationUsername);
}

std::string
Self::getTransactionReceiptForProductId(const std::string& productId) {
    return bridge_.call("getUnityPurchasingTransactionReceiptForProductId",
                        productId);
}
} // namespace store
} // namespace ee
