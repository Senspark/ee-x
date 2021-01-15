//
//  StoreAppleReceiptParser.cpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#include "ee/store/private/StoreAppleReceiptParser.hpp"

#include <ee/core/IMessageBridge.hpp>
#include "ee/core/PluginManager.hpp"
#include <ee/nlohmann/json.hpp>

#include "ee/store/private/StoreAppleInAppPurchaseReceipt.hpp"
#include "ee/store/private/StoreAppleReceipt.hpp"

namespace ee {
namespace store {
namespace {
const std::string kPrefix = "StoreBridge";
const auto kParseAppReceipt = kPrefix + "ParseAppReceipt";
} // namespace

void from_json(const nlohmann::json& json,
               std::shared_ptr<AppleInAppPurchaseReceipt>& value) {
    value = std::make_shared<AppleInAppPurchaseReceipt>(
        json["quantity"],                      //
        json["productId"],                     //
        json["transactionId"],                 //
        json["originalTransactionIdentifier"], //
        json["purchaseDate"],                  //
        json["originalPurchaseDate"],          //
        json["subscriptionExpirationDate"],    //
        json["cancellationDate"],              //
        json["isFreeTrial"],                   //
        json["productType"],                   //
        json["isIntroductoryPricePeriod"]);
}

void from_json(const nlohmann::json& json,
               std::shared_ptr<AppleReceipt>& value) {
    value = std::make_shared<AppleReceipt>( //
        json["inAppPurchaseReceipts"],      //
        json["bundleId"],                   //
        json["appVersion"],                 //
        json["expirationDate"],             //
        json["originalApplicationVersion"], //
        json["receiptCreationDate"]);
}

using Self = AppleReceiptParser;

Self::AppleReceiptParser()
    : bridge_(PluginManager::getBridge()) {}

std::shared_ptr<AppleReceipt> Self::parse(const std::string& receiptData) {
    if (receiptData.empty()) {
        return nullptr;
    }
    auto response = bridge_.call(kParseAppReceipt, receiptData);
    auto json = nlohmann::json::parse(response);
    return json;
}
} // namespace store
} // namespace ee
