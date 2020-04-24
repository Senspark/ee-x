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
namespace {
// clang-format off
const std::string kPrefix = "Store";

const auto kGetSkuDetails      = kPrefix + "_getSkuDetails";
const auto kGetPurchases       = kPrefix + "_getPurchases";
const auto kGetPurchaseHistory = kPrefix + "_getPurchaseHistory";
const auto kPurchase           = kPrefix + "_purchase";
const auto kConsume            = kPrefix + "_consume";
const auto kAcknowledge        = kPrefix + "_acknowledge";

const auto kOnGetSkuDetails      = kPrefix + "_onGetSkuDetails";
const auto kOnGetPurchases       = kPrefix + "_onGetPurchases";
const auto kOnGetPurchaseHistory = kPrefix + "_onGetPurchaseHistory";
const auto kOnPurchase           = kPrefix + "_onPurchase";
const auto kOnConsume            = kPrefix + "_onConsume";
const auto kOnAcknowledge        = kPrefix + "_onAcknowledge";
// clang-format on
} // namespace

using Self = Bridge;

Self::Bridge()
    : Self(Logger::getSystemLogger()) {}

Self::Bridge(const Logger& logger)
    : bridge_(MessageBridge::getInstance())
    , logger_(logger) {
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            // TODO.
            return "";
        },
        kOnGetSkuDetails);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            // TODO.
            return "";
        },
        kOnGetPurchases);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            // TODO.
            return "";
        },
        kOnGetPurchaseHistory);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            // TODO.
            return "";
        },
        kOnPurchase);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            // TODO.
            return "";
        },
        kOnConsume);
    bridge_.registerHandler(
        [this](const std::string& message) {
            auto json = nlohmann::json::parse(message);
            // TODO.
            return "";
        },
        kOnAcknowledge);
}

Self::~Bridge() = default;

void Self::destroy() {
    bridge_.deregisterHandler(kOnGetSkuDetails);
    bridge_.deregisterHandler(kOnGetPurchases);
    bridge_.deregisterHandler(kOnGetPurchaseHistory);
    bridge_.deregisterHandler(kOnPurchase);
    bridge_.deregisterHandler(kOnConsume);
    bridge_.deregisterHandler(kOnAcknowledge);
}

void Self::initialize() {
    // TODO.
}

Task<std::vector<SkuDetails>>
Self::getSkuDetails(SkuType type, const std::vector<std::string>& skuList) {
    // TODO.
    co_return std::vector<SkuDetails>();
}

Task<std::vector<Purchase>> Self::getPurchases(SkuType type) {
    // TODO.
    co_return std::vector<Purchase>();
}

Task<std::vector<PurchaseHistoryRecord>>
Self::getPurchaseHistory(SkuType type) {
    // TODO.
    co_return std::vector<PurchaseHistoryRecord>();
}

Task<Purchase> Self::purchase(const std::string& sku) {
    // TODO.
    co_return Purchase();
}

Task<bool> Self::consume(const std::string& purchaseToken) {
    // TODO.
    co_return false;
}

Task<bool> Self::acknowledge(const std::string& purchaseToken) {
    // TODO.
    co_return false;
}
} // namespace store
} // namespace ee
