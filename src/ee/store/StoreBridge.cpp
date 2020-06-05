//
//  Store.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "ee/store/StoreBridge.hpp"

#include <ee/nlohmann/json.hpp>

#include <ee/core/Logger.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/core/internal/MessageBridge.hpp>

#include "ee/store/Purchase.hpp"
#include "ee/store/PurchaseHistoryRecord.hpp"
#include "ee/store/SkuDetails.hpp"

namespace ee {
namespace store {
std::string toString(SkuType data) {
    if (data == SkuType::InApp) {
        return "inapp";
    }
    if (data == SkuType::Subscription) {
        return "subs";
    }
    assert(false);
    return "";
}

void to_json(nlohmann::json& json, SkuType data) {
    json = toString(data);
}

void from_json(const nlohmann::json& json, SkuDetails& data) {
    data.description = json.at("description");
    data.freeTrialPeriod = json.at("free_trial_period");
    data.price = json.at("price");
    data.priceAmountMicros = json.at("price_amount_micros");
    data.sku = json.at("sku");
    data.subscriptionPeriod = json.at("subscription_period");
    data.title = json.at("title");
}

void from_json(const nlohmann::json& json, Purchase& data) {
    data.packageName = json.at("package_name");
    data.purchaseState = json.at("purchase_state");
    data.purchaseToken = json.at("purchase_token");
    data.signature = json.at("signature");
    data.sku = json.at("sku");
    data.isAcknowledged = json.at("is_acknowledged");
    data.isAutoRenewing = json.at("is_auto_renewing");
}

void from_json(const nlohmann::json& json, PurchaseHistoryRecord& data) {
    data.purchaseTime = json.at("purchase_time");
    data.purchaseToken = json.at("purchase_token");
    data.signature = json.at("signature");
    data.sku = json.at("sku");
}

namespace {
// clang-format off
const std::string kPrefix = "Store";

const auto kGetSkuDetails      = kPrefix + "_getSkuDetails";
const auto kGetPurchases       = kPrefix + "_getPurchases";
const auto kGetPurchaseHistory = kPrefix + "_getPurchaseHistory";
const auto kPurchase           = kPrefix + "_purchase";
const auto kConsume            = kPrefix + "_consume";
const auto kAcknowledge        = kPrefix + "_acknowledge";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {}

Self::~Bridge() = default;

void Self::destroy() {}

void Self::initialize() {
    // TODO.
}

Task<std::optional<std::vector<SkuDetails>>>
Self::getSkuDetails(SkuType type, const std::vector<std::string>& skuList) {
    nlohmann::json json;
    json["sku_type"] = type;
    json["sku_list"] = skuList;
    auto response = co_await bridge_.callAsync(kGetSkuDetails, json.dump());
    auto responseJson = nlohmann::json::parse(response);
    auto successful = responseJson.at("successful").get<bool>();
    if (successful) {
        auto items = responseJson.at("item").get<std::vector<SkuDetails>>();
        co_return items;
    }
    auto error = responseJson.at("error").get<std::string>();
    logger_.debug("%s: error = %s", __PRETTY_FUNCTION__, error.c_str());
    co_return std::nullopt;
}

Task<std::optional<std::vector<Purchase>>> Self::getPurchases(SkuType type) {
    auto response = co_await bridge_.callAsync(kGetPurchases, toString(type));
    auto responseJson = nlohmann::json::parse(response);
    auto successful = responseJson.at("successful").get<bool>();
    if (successful) {
        auto items = responseJson.at("item").get<std::vector<Purchase>>();
        co_return items;
    }
    auto error = responseJson.at("error").get<std::string>();
    logger_.debug("%s: error = %s", __PRETTY_FUNCTION__, error.c_str());
    co_return std::nullopt;
}

Task<std::optional<std::vector<PurchaseHistoryRecord>>>
Self::getPurchaseHistory(SkuType type) {
    auto response =
        co_await bridge_.callAsync(kGetPurchaseHistory, toString(type));
    auto responseJson = nlohmann::json::parse(response);
    auto successful = responseJson.at("successful").get<bool>();
    if (successful) {
        auto items =
            responseJson.at("item").get<std::vector<PurchaseHistoryRecord>>();
        co_return items;
    }
    auto error = responseJson.at("error").get<std::string>();
    logger_.debug("%s: error = %s", __PRETTY_FUNCTION__, error.c_str());
    co_return std::nullopt;
}

Task<std::optional<Purchase>> Self::purchase(const std::string& sku) {
    auto response = co_await bridge_.callAsync(kPurchase, sku);
    auto responseJson = nlohmann::json::parse(response);
    auto successful = responseJson.at("successful").get<bool>();
    if (successful) {
        auto item = responseJson.at("item").get<Purchase>();
        co_return item;
    }
    auto error = responseJson.at("error").get<std::string>();
    logger_.debug("%s: error = %s", __PRETTY_FUNCTION__, error.c_str());
    co_return std::nullopt;
}

Task<bool> Self::consume(const std::string& purchaseToken) {
    auto response = co_await bridge_.callAsync(kConsume, purchaseToken);
    auto responseJson = nlohmann::json::parse(response);
    auto successful = responseJson.at("successful").get<bool>();
    co_return successful;
}

Task<bool> Self::acknowledge(const std::string& purchaseToken) {
    auto response = co_await bridge_.callAsync(kAcknowledge, purchaseToken);
    auto responseJson = nlohmann::json::parse(response);
    auto successful = responseJson.at("successful").get<bool>();
    co_return successful;
}
} // namespace store
} // namespace ee
