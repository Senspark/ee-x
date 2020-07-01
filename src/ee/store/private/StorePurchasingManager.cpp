//
//  StorePurchasingManager.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/private/StorePurchasingManager.hpp"

#include <ee/core/Logger.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/store/StoreIStore.hpp"
#include "ee/store/StoreITransactionLog.hpp"
#include "ee/store/StoreInitializationFailureReason.hpp"
#include "ee/store/StoreProduct.hpp"
#include "ee/store/StoreProductCollection.hpp"
#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/StoreProductDescription.hpp"
#include "ee/store/StoreProductMetadata.hpp"
#include "ee/store/StoreProductType.hpp"
#include "ee/store/StorePurchaseFailureDescription.hpp"
#include "ee/store/StorePurchaseFailureReason.hpp"
#include "ee/store/StorePurchaseProcessingResult.hpp"
#include "ee/store/private/StoreIInternalStoreListener.hpp"

namespace ee {
namespace store {
using Self = PurchasingManager;

Self::PurchasingManager(const std::shared_ptr<ITransactionLog>& transactionLog,
                        const Logger& logger,
                        const std::shared_ptr<IStore>& store,
                        const std::string& storeName)
    : transactionLog_(transactionLog)
    , logger_(logger)
    , store_(store)
    , storeName_(storeName) {
    useTransactionLog_ = true;
    initialized_ = false;
}

bool Self::useTransactionLog() const {
    return useTransactionLog_;
}

void Self::useTransactionLog(bool enabled) {
    useTransactionLog_ = enabled;
}

void Self::initiatePurchase(const std::shared_ptr<Product>& product) {
    if (product == nullptr) {
        logger_.debug("Trying to purchase null product");
    } else if (not product->availableToPurchase()) {
        listener_->onPurchaseFailed(product,
                                    PurchaseFailureReason::ProductUnavailable);
    } else {
        store_->purchase(product->definition());
        logger_.debug("purchase(%s)", product->definition()->id().c_str());
    }
}

void Self::initiatePurchase(const std::string& productId) {
    auto&& product = products_->withId(productId);
    if (product == nullptr) {
        logger_.warn("Unable to purchase unknown product with id: %s",
                     productId.c_str());
    }
    initiatePurchase(product);
}

void Self::confirmPendingPurchase(const std::shared_ptr<Product>& product) {
    if (product == nullptr) {
        logger_.debug("Unable to confirm purchase with null product");
    } else if (product->transactionId().empty()) {
        logger_.debug("Unable to confirm purchase; product has missing or "
                      "empty transactionId");
    } else {
        if (useTransactionLog()) {
            transactionLog_->record(product->transactionId());
        }
        store_->finishTransaction(product->definition(),
                                  product->transactionId());
    }
}

std::shared_ptr<ProductCollection> Self::products() const {
    return products_;
}

void Self::onPurchaseSucceeded(const std::string& storeSpecificId,
                               const std::string& receipt,
                               const std::string& transactionId) {
    auto&& product = products_->withId(storeSpecificId);
    if (product == nullptr) {
        product = std::make_shared<Product>(
            std::make_shared<ProductDefinition>(storeSpecificId,
                                                ProductType::NonConsumable),
            std::make_shared<ProductMetadata>());
    }
    product->receipt_ = FormatUnifiedReceipt(receipt, transactionId);
    product->transactionId_ = transactionId;
    processPurchaseIfNew(product);
}

void Self::onSetupFailed(InitializationFailureReason reason) {
    if (initialized_) {
        return;
    } else {
        listener_->onInitializeFailed(reason);
    }
}

void Self::onPurchaseFailed(const PurchaseFailureDescription& description) {
    auto&& product = products_->withStoreSpecificId(description.productId);
    if (product == nullptr) {
        logger_.error("Failed to purchase unknown product %s",
                      description.productId.c_str());
    } else {
        listener_->onPurchaseFailed(product, description.reason);
    }
}

void Self::onProductsRetrieved(
    const std::vector<std::shared_ptr<ProductDescription>>& products) {
    std::set<std::shared_ptr<Product>> productSet;
    for (auto&& description : products) {
        auto&& product =
            products_->withStoreSpecificId(description->storeSpecificId());
        if (product == nullptr) {
            product = std::make_shared<Product>(
                std::make_shared<ProductDefinition>(
                    description->storeSpecificId(),
                    description->storeSpecificId(), description->type()),
                description->metadata());
            productSet.insert(product);
        }
        product->availableToPurchase_ = true;
        product->metadata_ = description->metadata();
        product->transactionId_ = description->transactionId();
        if (not description->receipt().empty()) {
            product->receipt_ = FormatUnifiedReceipt(
                description->receipt(), description->transactionId());
        }
    }
    if (not productSet.empty()) {
        // products_->addProducts(/* FIXME: productSet */)
    }
    checkForInitialization();
    for (auto&& product : products_->all()) {
        if (not product->receipt().empty() &&
            not product->transactionId().empty()) {
            processPurchaseIfNew(product);
        }
    }
}

void Self::processPurchaseIfNew(const std::shared_ptr<Product>& product) {
    if (useTransactionLog() &&
        transactionLog_->hasRecordOf(product->transactionId())) {
        logger_.debug("Already recorded transaction %s",
                      product->transactionId().c_str());
        store_->finishTransaction(product->definition(),
                                  product->transactionId());
    } else {
        if (listener_->processPurchase(product) !=
            PurchaseProcessingResult::Complete) {
            return;
        }
        confirmPendingPurchase(product);
    }
}

void Self::checkForInitialization() {
    if (not initialized_) {
        auto flag = false;
        for (auto&& product : products_->all()) {
            if (not product->availableToPurchase()) {
                logger_.warn("Unavailable product %s - %s",
                             product->definition()->id().c_str(),
                             product->definition()->storeSpecificId().c_str());
            } else {
                flag = true;
            }
        }
        if (flag) {
            listener_->onInitialized(shared_from_this());
        } else {
            onSetupFailed(InitializationFailureReason::NoProductsAvailable);
        }
        initialized_ = true;
    }
}

void Self::initialize(
    const std::shared_ptr<IInternalStoreListener>& listener,
    const std::set<std::shared_ptr<ProductDefinition>>& products) {
    listener_ = listener;
    store_->initialize(shared_from_this());

    std::vector<std::shared_ptr<Product>> items;
    std::transform(products.cbegin(), products.cend(),
                   std::back_inserter(items), [](auto&& item) {
                       return std::make_shared<Product>(
                           item, std::make_shared<ProductMetadata>());
                   });
    products_ = std::make_shared<ProductCollection>(items);
    std::vector<std::shared_ptr<ProductDefinition>> list(products.cbegin(),
                                                         products.cend());
    store_->retrieveProducts(list);
}

std::string Self::FormatUnifiedReceipt(const std::string& platformReceipt,
                                       const std::string& transactionId) {
    nlohmann::json json;
    json["Store"] = storeName_;
    json["TransactionID"] = transactionId;
    json["Payload"] = platformReceipt;
    return json.dump();
}
} // namespace store
} // namespace ee
