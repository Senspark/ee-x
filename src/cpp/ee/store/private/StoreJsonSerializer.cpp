//
//  StoreJsonSerializer.cpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#include "ee/store/private/StoreJsonSerializer.hpp"

#include <ee/nlohmann/json.hpp>

#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/StoreProductMetadata.hpp"
#include "ee/store/private/StoreProductDescription.hpp"
#include "ee/store/private/StorePurchaseFailureDescription.hpp"
#include "ee/store/private/StorePurchaseFailureReason.hpp"

namespace ee {
namespace store {
using Self = JsonSerializer;

void to_json(nlohmann::json& json,
             const std::shared_ptr<ProductDefinition>& value) {
    json["id"] = value->id();
    json["storeSpecificId"] = value->storeSpecificId();
    json["type"] = value->type();
}

void to_json(nlohmann::json& json,
             const std::shared_ptr<ProductMetadata>& value) {
    json["localizedPriceString"] = value->localizedPriceString();
    json["localizedTitle"] = value->localizedTitle();
    json["localizedDescription"] = value->localizedDescription();
    json["isoCurrencyCode"] = value->isoCurrencyCode();
    json["localizedPrice"] = value->localizedPrice();
}

void from_json(const nlohmann::json& json,
               std::shared_ptr<ProductMetadata>& value) {
    value = std::make_shared<ProductMetadata>(json["localizedPriceString"], //
                                              json["localizedTitle"],       //
                                              json["localizedDescription"],
                                              json["isoCurrencyCode"],
                                              json["localizedPrice"]);
}

void to_json(nlohmann::json& json,
             const std::shared_ptr<ProductDescription>& value) {
    json["storeSpecificId"] = value->storeSpecificId();
    json["metadata"] = value->metadata();
    json["receipt"] = value->receipt();
    json["transactionId"] = value->transactionId();
}

void from_json(const nlohmann::json& json,
               std::shared_ptr<ProductDescription>& value) {
    value = std::make_shared<ProductDescription>(
        json["storeSpecificId"],   //
        json["metadata"],          //
        json.value("receipt", ""), //
        json.value("transactionId", ""),
        json.value("type", ProductType::NonConsumable));
}

std::string Self::serializeProductDefinition(
    const std::shared_ptr<ProductDefinition>& product) {
    nlohmann::json json = product;
    return json.dump();
}

std::string Self::serializeProductDefinitions(
    const std::vector<std::shared_ptr<ProductDefinition>>& products) {
    nlohmann::json json;
    for (auto&& product : products) {
        json.push_back(product);
    }
    return json.dump();
}

std::string Self::serializeProductDescription(
    const std::shared_ptr<ProductDescription>& product) {
    nlohmann::json json = product;
    return json.dump();
}

std::string Self::serializeProductDescriptions(
    const std::vector<std::shared_ptr<ProductDescription>>& products) {
    nlohmann::json json;
    for (auto&& product : products) {
        json.push_back(product);
    }
    return json.dump();
}

std::vector<std::shared_ptr<ProductDescription>>
Self::deserializeProductDescriptions(const std::string& str) {
    auto json = nlohmann::json::parse(str);
    return json;
}

std::map<std::string, std::string>
Self::deserializeSubscriptionDescriptions(const std::string& json) {
    auto objectList = nlohmann::json::parse(json);
    std::map<std::string, std::string> dictionary1;
    for (auto&& dictionary2 : objectList) {
        auto&& dic = dictionary2["metadata"];
        auto key = dictionary2["storeSpecificId"].get<std::string>();
        nlohmann::json dictionary3;
        dictionary3["introductoryPrice"] = dic["introductoryPrice"];
        dictionary3["introductoryPriceLocale"] = dic["introductoryPriceLocale"];
        dictionary3["introductoryPriceNumberOfPeriods"] =
            dic["introductoryPriceNumberOfPeriods"];
        dictionary3["numberOfUnits"] = dic["numberOfUnits"];
        dictionary3["unit"] = dic["unit"];
        if (not dictionary3["numberOfUnits"].is_null() &&
            dictionary3["unit"].is_null()) {
            dictionary3["unit"] = 0;
        }
        dictionary1[key] = dictionary3.dump();
    }
    return dictionary1;
}

std::map<std::string, std::string>
Self::deserializeProductDetails(const std::string& json) {
    auto objectList = nlohmann::json::parse(json);
    std::map<std::string, std::string> dictionary1;
    for (auto&& dictionary2 : objectList) {
        auto&& dic = dictionary2["metadata"];
        auto key = dictionary2["storeSpecificId"].get<std::string>();
        nlohmann::json dictionary3;
        dictionary3["subscriptionNumberOfUnits"] =
            dic["subscriptionNumberOfUnits"];
        dictionary3["subscriptionPeriodUnit"] = dic["subscriptionPeriodUnit"];
        dictionary3["localizedPrice"] = dic["localizedPrice"];
        dictionary3["isoCurrencyCode"] = dic["isoCurrencyCode"];
        dictionary3["localizedPriceString"] = dic["localizedPriceString"];
        dictionary3["localizedTitle"] = dic["localizedTitle"];
        dictionary3["localizedDescription"] = dic["localizedDescription"];
        dictionary3["introductoryPrice"] = dic["introductoryPrice"];
        dictionary3["introductoryPriceLocale"] = dic["introductoryPriceLocale"];
        dictionary3["introductoryPriceNumberOfPeriods"] =
            dic["introductoryPriceNumberOfPeriods"];
        dictionary3["numberOfUnits"] = dic["numberOfUnits"];
        dictionary3["unit"] = dic["unit"];
        if (not dictionary3["subscriptionNumberOfUnits"].is_null() &&
            dictionary3["subscriptionPeriodUnit"].is_null()) {
            dictionary3["subscriptionPeriodUnit"] = 0;
        }
        if (not dictionary3["numberOfUnits"].is_null() &&
            dictionary3["unit"].is_null()) {
            dictionary3["unit"] = "0";
        }
        dictionary1[key] = dictionary3.dump();
    }
    return dictionary1;
}

PurchaseFailureDescription
Self::deserializeFailureReason(const std::string& json) {
    using Enum = PurchaseFailureReason;

#if defined(EE_X_ANDROID)
    static std::map<int, Enum> reasonMap     //
            {{0, Enum::PurchasingUnavailable},   //
             {1, Enum::ExistingPurchasePending}, //
             {2, Enum::ProductUnavailable},      //
             {3, Enum::SignatureInvalid},        //
             {4, Enum::UserCancelled},           //
             {5, Enum::PaymentDeclined},         //
             {6, Enum::DuplicateTransaction}};
#endif // defined(EE_X_ANDROID)
#if defined(EE_X_IOS)
    static std::map<std::string, Enum> reasonMap{
        {"PurchasingUnavailable", Enum::PurchasingUnavailable},
        {"ExistingPurchasePending", Enum::ExistingPurchasePending},
        {"ProductUnavailable", Enum::ProductUnavailable},
        {"SignatureInvalid", Enum::SignatureInvalid},
        {"UserCancelled", Enum::UserCancelled},
        {"PaymentDeclined", Enum::PaymentDeclined},
        {"DuplicateTransaction", Enum::DuplicateTransaction}};
#endif // defined(EE_X_IOS)

    auto dic = nlohmann::json::parse(json);
    auto reason = PurchaseFailureReason::Unknown;
    try {
        auto iter = reasonMap.find(dic["reason"]);
        if (iter != reasonMap.cend()) {
            reason = iter->second;
        }
    }
    catch (...) {
        reason = PurchaseFailureReason::Unknown;
    }
    return PurchaseFailureDescription(dic["productId"], reason, dic["message"]);
}
} // namespace store
} // namespace ee
