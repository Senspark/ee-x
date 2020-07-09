//
//  StoreIosStoreBindings.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/private/StoreIosStoreBindings.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/Thread.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace store {
namespace {
const std::string kPrefix = "StoreBridge";
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
            Thread::runOnLibraryThread([this, message] { //
                auto json = nlohmann::json::parse(message);
                if (purchasingCallback_) {
                    purchasingCallback_(json["subject"], json["payload"],
                                        json["receipt"], json["transactionId"]);
                }
            });
            return "";
        },
        kCallback);
}

Self::~IosStoreBindings() {
    bridge_.deregisterHandler(kCallback);
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
