//
//  Store.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "ee/store/StoreBridge.hpp"
#include "ee/Macro.hpp"
#include "ee/core/Utils.hpp"
#include "ee/core/MessageBridge.hpp"

#include <ee/nlohmann/json.hpp>

namespace ee {
namespace store {
namespace {
// clang-format off
constexpr auto k__initialize                  = "Store_initialize";
constexpr auto k__can_purchase                = "Store_canPurchase";
constexpr auto k__purchase                    = "Store_purchase";
constexpr auto k__restore_transactions        = "Store_restoreTransactions";
constexpr auto k__request_products            = "Store_requestProducts";
constexpr auto k__request_products_succeeded  = "Store_requestProductSucceeded";
constexpr auto k__request_products_failed     = "Store_requestProductFailed";
constexpr auto k__restore_purchases_succeeded = "Store_restorePurchasesSucceeded";
constexpr auto k__restore_purchases_failed    = "Store_restorePurchasesFailed";
constexpr auto k__transaction_succeeded       = "Store_transactionSucceeded";
constexpr auto k__transaction_failed          = "Store_transactionFailed";
constexpr auto k__transaction_restored        = "Store_transactionRestored";
// clang-format on

// clang-format off
constexpr auto k__title             = "title";
constexpr auto k__description       = "description";
constexpr auto k__price             = "price";
constexpr auto k__product_id        = "product_id";
constexpr auto k__transaction_id    = "transaction_id";
constexpr auto k__currency_symbol   = "currency_symbol";
constexpr auto k__currency_code     = "currency_code";
constexpr auto k__error_code        = "error_code";
// clang-format on
} // namespace

using Self = Store;

Self::Store() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            return "";
        },
        k__request_products_succeeded);
    bridge.registerHandler(
        [this](const std::string& message) {
            auto errorCode = std::stoi(message);
            onRequestProductFailed(errorCode);
            return "";
        },
        k__request_products_failed);
    bridge.registerHandler(
        [this](const std::string& message) {
            onRestorePurchasesSucceeded();
            return "";
        },
        k__restore_purchases_succeeded);
    bridge.registerHandler(
        [this](const std::string& message) {
            auto errorCode = std::stoi(message);
            onRestorePurchasesFailed(errorCode);
            return "";
        },
        k__restore_purchases_failed);
    bridge.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            std::string productId = json[k__product_id];
            std::string transactionId = json[k__transaction_id];
            onTransactionSucceeded(productId, transactionId);
            return "";
        },
        k__transaction_succeeded);
    bridge.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            std::string productId = json[k__product_id];
            int errorCode = json[k__error_code];
            onTransactionFailed(productId, errorCode);
            return "";
        },
        k__transaction_failed);
    bridge.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            std::string productId = json[k__product_id];
            std::string transactionId = json[k__transaction_id];
            onTransactionRestored(productId, transactionId);
            return "";
        },
        k__transaction_restored);
}

Self::~Store() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.deregisterHandler(k__request_products_succeeded);
    bridge.deregisterHandler(k__request_products_failed);
    bridge.deregisterHandler(k__restore_purchases_succeeded);
    bridge.deregisterHandler(k__restore_purchases_failed);
    bridge.deregisterHandler(k__transaction_succeeded);
    bridge.deregisterHandler(k__transaction_failed);
    bridge.deregisterHandler(k__transaction_restored);
}

void Self::initialize(const std::string& publicKey) {
#ifdef EE_X_ANDROID
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__initialize, publicKey);
#endif // EE_X_ANDROID
}

bool Self::canPurchase() const {
    auto&& bridge = MessageBridge::getInstance();
    auto response = bridge.call(k__can_purchase);
    return core::toBool(response);
}

void Self::purchase(const std::string& productId) {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__purchase, productId);
}

void Self::restoreTransactions() {
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__restore_transactions);
}

void Self::requestProducts(const std::vector<std::string>& productIds) {
    nlohmann::json json = productIds;
    auto&& bridge = MessageBridge::getInstance();
    bridge.call(k__request_products, json.dump());
}
} // namespace store
} // namespace ee
