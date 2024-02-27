//
//  StoreAndroidJavaStore.cpp
//  Pods
//
//  Created by eps on 7/8/20.
//

#include "ee/store/private/StoreAndroidJavaStore.hpp"

#include <ee/core/IMessageBridge.hpp>
#include <ee/core/NoAwait.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

namespace ee {
namespace store {
namespace {
const std::string kPrefix = "StoreBridge";
const auto kRetrieveProducts = kPrefix + "RetrieveProducts";
const auto kPurchase = kPrefix + "Purchase";
const auto kFinishTransaction = kPrefix + "FinishTransaction";
const auto kOnSetupFailed = kPrefix + "OnSetupFailed";
const auto kOnProductsRetrieved = kPrefix + "OnProductsRetrieved";
const auto kOnPurchaseSucceeded = kPrefix + "OnPurchaseSucceeded";
const auto kOnPurchaseFailed = kPrefix + "OnPurchaseFailed";
const auto kGetProductJson = kPrefix + "GetProductJson";
const auto kRestoreTransactions = kPrefix + "RestoreTransactions";
const auto kFinishAdditionalTransaction = kPrefix + "FinishAdditionalTransaction";
const auto kSetObfuscatedAccountId = kPrefix + "SetObfuscatedAccountId";
} // namespace

using Self = AndroidJavaStore;

Self::AndroidJavaStore(IMessageBridge& bridge,
                       const std::shared_ptr<IUnityCallback>& forwardTo)
    : bridge_(bridge)
    , forwardTo_(forwardTo) {
    bridge_.registerHandler(
        [this](const std::string& message) {
            onSetupFailed(message);
            return "";
        },
        kOnSetupFailed);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onProductsRetrieved(message);
            return "";
        },
        kOnProductsRetrieved);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            onPurchaseSucceeded(json["id"], json["receipt"],
                                json["transactionId"]);
            return "";
        },
        kOnPurchaseSucceeded);
    bridge_.registerHandler(
        [this](const std::string& message) {
            onPurchaseFailed(message);
            return "";
        },
        kOnPurchaseFailed);
}

Self::~AndroidJavaStore() {
    bridge_.deregisterHandler(kOnSetupFailed);
    bridge_.deregisterHandler(kOnProductsRetrieved);
    bridge_.deregisterHandler(kOnPurchaseSucceeded);
    bridge_.deregisterHandler(kOnPurchaseFailed);
}

void Self::retrieveProducts(const std::string& json) {
    bridge_.call(kRetrieveProducts, json);
}

void Self::purchase(const std::string& productJson) {
    bridge_.call(kPurchase, productJson);
}

void Self::finishTransaction(const std::string& productJson,
                             const std::string& transactionId) {
    nlohmann::json json;
    json["productJson"] = productJson;
    json["transactionId"] = transactionId;
    bridge_.call(kFinishTransaction, json.dump());
}

void Self::onSetupFailed(const std::string& json) {
    forwardTo_->onSetupFailed(json);
}

void Self::onProductsRetrieved(const std::string& json) {
    forwardTo_->onProductsRetrieved(json);
}

void Self::onPurchaseSucceeded(const std::string& id,
                               const std::string& receipt,
                               const std::string& transactionId) {
    forwardTo_->onPurchaseSucceeded(id, receipt, transactionId);
}

void Self::onPurchaseFailed(const std::string& json) {
    forwardTo_->onPurchaseFailed(json);
}

void Self::setPublicKey(const std::string& key) {
    // Empty in Unity.
}

std::map<std::string, std::string> Self::getProductJsonDictionary() const {
    auto response = bridge_.call(kGetProductJson);
    auto json = nlohmann::json::parse(response);
    return json;
}

void Self::restoreTransactions(const std::function<void(bool)>& callback) {
    noAwait([this, callback]() -> Task<> {
        auto response = co_await bridge_.callAsync(kRestoreTransactions);
        callback(core::toBool(response));
    });
}

void Self::finishAdditionalTransaction(const std::string& productId,
                                       const std::string& transactionId) {
    nlohmann::json json;
    json["productId"] = productId;
    json["transactionId"] = transactionId;
    bridge_.call(kFinishAdditionalTransaction, json.dump());
}

void Self::setObfuscationAccountId(const std::string &accountId) {
    bridge_.call(kSetObfuscatedAccountId, accountId);
}
} // namespace store
} // namespace ee
