//
//  StoreSubscriptionManager.cpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#include "ee/store/StoreSubscriptionManager.hpp"

#include <ee/nlohmann/json.hpp>

#include "ee/store/StoreAppleInAppPurchaseReceipt.hpp"
#include "ee/store/StoreAppleReceipt.hpp"
#include "ee/store/StoreProduct.hpp"
#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/StoreSubscriptionInfo.hpp"
#include "ee/store/private/StoreAppleReceiptParser.hpp"

namespace ee {
namespace store {
using Self = SubscriptionManager;

Self::SubscriptionManager(const std::shared_ptr<Product>& product,
                          const std::string& introJson)
    : Self(product->receipt(), product->definition()->storeSpecificId(),
           introJson) {}

Self::SubscriptionManager(const std::string& receipt, const std::string& id,
                          const std::string& introJson)
    : receipt_(receipt)
    , productId_(id)
    , introJson_(introJson) {}

std::shared_ptr<SubscriptionInfo> Self::getSubscriptionInfo() {
    if (receipt_.empty()) {
        assert(false);
        return nullptr;
    }
    auto&& dictionary = nlohmann::json::parse(receipt_);
    auto&& store = dictionary["Store"].get<std::string>();
    auto&& payload = dictionary["Payload"].get<std::string>();
    if (store == "GooglePlay") {
        return getGooglePlayStoreSubInfo(payload);
    }
    if (productId_.empty()) {
        assert(false);
        return nullptr;
    }
    return getAppleAppStoreSubInfo(payload, productId_);
}

std::shared_ptr<SubscriptionInfo>
Self::getAppleAppStoreSubInfo(const std::string& payload,
                              const std::string& productId) {
    auto receipt = AppleReceiptParser().parse(payload);
    if (receipt == nullptr) {
        return nullptr;
    }
    std::vector<std::shared_ptr<AppleInAppPurchaseReceipt>> all;
    for (auto&& r : receipt->inAppPurchaseReceipts()) {
        if (r->productId() == productId) {
            all.push_back(r);
        }
    }
    if (all.empty()) {
        return nullptr;
    }
    std::stable_sort(all.begin(), all.end(),
                     [](const std::shared_ptr<AppleInAppPurchaseReceipt>& b,
                        const std::shared_ptr<AppleInAppPurchaseReceipt>& a) {
                         return a->purchaseDate() < b->purchaseDate();
                     });
    return std::make_shared<SubscriptionInfo>(all[0], introJson_);
}

std::shared_ptr<SubscriptionInfo>
Self::getGooglePlayStoreSubInfo(const std::string& payload) {
    auto&& dictionary1 = nlohmann::json::parse(payload);
    auto&& skuDetails = dictionary1["skuDetails"].get<std::string>();
    auto&& purchaseHistorySupported =
        dictionary1["isPurchaseHistorySupported"].get<bool>();
    auto&& dictionary2 = dictionary1["json"];
    auto&& isAutoRenewing = dictionary2["autoReviewing"].get<bool>();
    auto&& purchaseDate = dictionary2["purchaseTime"].get<int>();
    auto&& dictionary3 = dictionary2["developerPayload"];
    auto&& isFreeTrial = dictionary3["is_free_trial"].get<bool>();
    auto&& hasIntroductoryPriceTrial =
        dictionary3["has_introductory_price_trial"].get<bool>();
    std::string updateMetadata;
    if (dictionary3["is_updated"].get<bool>()) {
        updateMetadata = dictionary3["update_subscription_metadata"];
    }
    return std::make_shared<SubscriptionInfo>(
        skuDetails, isAutoRenewing, purchaseDate, isFreeTrial,
        hasIntroductoryPriceTrial, purchaseHistorySupported, updateMetadata);
}
} // namespace store
} // namespace ee
