//
//  StoreIosStoreBindings.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "StoreIosStoreBindings.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace store {
namespace {
const std::string kPrefix = "Store";
const auto kGetAppReceipt = kPrefix + "GetAppReceipt";
const auto kCanMakePayments = kPrefix + "CanMakePayments";
const auto kGetSimulateAskToBuy = kPrefix + "GetSimulateAskToBuy";
const auto kSetSimulateAskToBuy = kPrefix + "SetSimulateAskToBuy";
const auto kRetrieveProducts = kPrefix + "RetrieveProducts";
const auto kPurchase = kPrefix + "Purchase";
const auto kFinishTransaction = kPrefix + "FinishTransaction";
const auto kRestoreTransactions = kPrefix + "RestoreTransactions";
const auto kRefreshAppReceipt = kPrefix + "RefreshAppReceipt";
const auto kAddTransactionObserver = kPrefix + "AddTransactionObserver";
const auto kGetTransactionReceiptForProductId =
    kPrefix + "GetTransactionReceiptForProductId";
const auto kCallback = kPrefix + "Callback";
} // namespace

using Self = IosStoreBindings;

Self::IosStoreBindings(IMessageBridge& bridge)
    : bridge_(bridge) {
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            std::string subject = json["subject"];
            std::string payload = json["payload"];
            std::string receipt = json["receipt"];
            std::string transactionId = json["transactionId"];
            if (purchasingCallback_) {
                purchasingCallback_(subject, payload, receipt, transactionId);
            }
            return "";
        },
        kCallback);

    // FIXME: leak handler.
}

void Self::setPurchasingCallback(const PurchasingCallback& callback) {
    purchasingCallback_ = callback;
}

std::string Self::appReceipt() const {
    return bridge_.call(kGetAppReceipt);
}

bool Self::canMakePayments() const {
    return core::toBool(bridge_.call(kCanMakePayments));
}

bool Self::simulateAskToBuy() const {
    return core::toBool(bridge_.call(kGetSimulateAskToBuy));
}

void Self::simulateAskToBuy(bool enabled) {
    bridge_.call(kSetSimulateAskToBuy, core::toString(enabled));
}

void Self::retrieveProducts(const std::string& json) {
    bridge_.call(kRetrieveProducts, json);
}

void Self::purchase(const std::string& productJson) {
    bridge_.call(kPurchase, productJson);
}

void Self::finishTransaction(const std::string& productJson,
                             const std::string& transactionId) {
    bridge_.call(kFinishTransaction, transactionId);
}

void Self::restoreTransactions() {
    bridge_.call(kRestoreTransactions);
}

void Self::refreshAppReceipt() {
    bridge_.call(kRefreshAppReceipt);
}

void Self::addTransactionObserver() {
    bridge_.call(kAddTransactionObserver);
}

std::string
Self::getTransactionReceiptForProductId(const std::string& productId) {
    return bridge_.call(kGetTransactionReceiptForProductId, productId);
}
} // namespace store
} // namespace ee
