//
//  StoreJsonStore.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/private/StoreJsonStore.hpp"

#include <ee/core/Logger.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/store/StoreProductCollection.hpp"
#include "ee/store/private/StoreINativeStore.hpp"
#include "ee/store/private/StoreIStoreCallback.hpp"
#include "ee/store/private/StoreInitializationFailureReason.hpp"
#include "ee/store/private/StoreJsonSerializer.hpp"
#include "ee/store/private/StorePurchaseFailureDescription.hpp"
#include "ee/store/private/StoreStoreSpecificPurchaseErrorCode.hpp"

namespace ee {
namespace store {
using Self = JsonStore;

Self::JsonStore()
    : logger_(Logger::getSystemLogger()) {
    isRefreshing_ = false;
    isFirstTimeRetrievingProducts_ = true;
    lastPurchaseErrorCode_ = StoreSpecificPurchaseErrorCode::Unknown;
}

void Self::setNativeStore(const std::shared_ptr<INativeStore>& native) {
    store_ = native;
}

void Self::setModule(const std::shared_ptr<StandardPurchasingModule>& module) {
    if (module == nullptr) {
        return;
    }
    module_ = module;
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
    using Enum = InitializationFailureReason;
    static std::map<std::string, Enum> strToEnum{
        {"PurchasingUnavailable", Enum::PurchasingUnavailable},
        {"NoProductsAvailable", Enum::NoProductsAvailable},
        {"AppNotKnown", Enum::AppNotKnown},
    };
    unity_->onSetupFailed(strToEnum.at(json));
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
    lastPurchaseErrorCode_ = parseStoreSpecificPurchaseErrorCode(json);
    unity_->onPurchaseFailed(failure);
}

PurchaseFailureDescription Self::getLastPurchaseFailureDescription() const {
    return *lastPurchaseFailureDescription_;
}

StoreSpecificPurchaseErrorCode
Self::getLastStoreSpecificPurchaseErrorCode() const {
    return lastPurchaseErrorCode_;
}

StoreSpecificPurchaseErrorCode
Self::parseStoreSpecificPurchaseErrorCode(const std::string& json) {
    using Enum = StoreSpecificPurchaseErrorCode;
    std::map<std::string, Enum> strToEnum = {
        {"SKErrorUnknown", Enum::SKErrorUnknown},
        {"SKErrorClientInvalid", Enum::SKErrorClientInvalid},
        {"SKErrorPaymentCancelled", Enum::SKErrorPaymentCancelled},
        {"SKErrorPaymentInvalid", Enum::SKErrorPaymentInvalid},
        {"SKErrorPaymentNotAllowed", Enum::SKErrorPaymentNotAllowed},
        {"SKErrorStoreProductNotAvailable",
         Enum::SKErrorStoreProductNotAvailable},
        {"SKErrorCloudServicePermissionDenied",
         Enum::SKErrorCloudServicePermissionDenied},
        {"SKErrorCloudServiceNetworkConnectionFailed",
         Enum::SKErrorCloudServiceNetworkConnectionFailed},
        {"SKErrorCloudServiceRevoked", Enum::SKErrorCloudServiceRevoked},
        {"BILLING_RESPONSE_RESULT_OK", Enum::BILLING_RESPONSE_RESULT_OK},
        {"BILLING_RESPONSE_RESULT_USER_CANCELED",
         Enum::BILLING_RESPONSE_RESULT_USER_CANCELED},
        {"BILLING_RESPONSE_RESULT_SERVICE_UNAVAILABLE",
         Enum::BILLING_RESPONSE_RESULT_SERVICE_UNAVAILABLE},
        {"BILLING_RESPONSE_RESULT_BILLING_UNAVAILABLE",
         Enum::BILLING_RESPONSE_RESULT_BILLING_UNAVAILABLE},
        {"BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE",
         Enum::BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE},
        {"BILLING_RESPONSE_RESULT_DEVELOPER_ERROR",
         Enum::BILLING_RESPONSE_RESULT_DEVELOPER_ERROR},
        {"BILLING_RESPONSE_RESULT_ERROR", Enum::BILLING_RESPONSE_RESULT_ERROR},
        {"BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED",
         Enum::BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED},
        {"BILLING_RESPONSE_RESULT_ITEM_NOT_OWNED",
         Enum::BILLING_RESPONSE_RESULT_ITEM_NOT_OWNED},
        {"IABHELPER_ERROR_BASE", Enum::IABHELPER_ERROR_BASE},
        {"IABHELPER_REMOTE_EXCEPTION", Enum::IABHELPER_REMOTE_EXCEPTION},
        {"IABHELPER_BAD_RESPONSE", Enum::IABHELPER_BAD_RESPONSE},
        {"IABHELPER_VERIFICATION_FAILED", Enum::IABHELPER_VERIFICATION_FAILED},
        {"IABHELPER_SEND_INTENT_FAILED", Enum::IABHELPER_SEND_INTENT_FAILED},
        {"IABHELPER_USER_CANCELLED", Enum::IABHELPER_USER_CANCELLED},
        {"IABHELPER_UNKNOWN_PURCHASE_RESPONSE",
         Enum::IABHELPER_UNKNOWN_PURCHASE_RESPONSE},
        {"IABHELPER_MISSING_TOKEN", Enum::IABHELPER_MISSING_TOKEN},
        {"IABHELPER_UNKNOWN_ERROR", Enum::IABHELPER_UNKNOWN_ERROR},
        {"IABHELPER_SUBSCRIPTIONS_NOT_AVAILABLE",
         Enum::IABHELPER_SUBSCRIPTIONS_NOT_AVAILABLE},
        {"IABHELPER_INVALID_CONSUMPTION", Enum::IABHELPER_INVALID_CONSUMPTION},
        {"Amazon_ALREADY_PURCHASED", Enum::Amazon_ALREADY_PURCHASED},
        {"Amazon_FAILED", Enum::Amazon_FAILED},
        {"Amazon_INVALID_SKU", Enum::Amazon_INVALID_SKU},
        {"Amazon_NOT_SUPPORTED", Enum::Amazon_NOT_SUPPORTED},
    };

    auto result = StoreSpecificPurchaseErrorCode::Unknown;
    try {
        auto dic = nlohmann::json::parse(json);
        if (not dic.is_object()) {
            return result;
        }
        auto str = dic["storeSpecificErrorCode"];
        auto iter = strToEnum.find(str);
        if (iter != strToEnum.cend()) {
            result = iter->second;
        }
    } catch (const std::exception& ex) {
        // Ignored.
    }
    return result;
}

void JsonStore::setObfuscationAccountId(const std::string &obfuscationAccountId) {
    store_->setObfuscationAccountId(obfuscationAccountId);
}
} // namespace store
} // namespace ee
