//
//  Store.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "ee/store/private/StoreBridge.hpp"

#include <ee/core/ILogger.hpp>
#include <ee/core/IMessageBridge.hpp>
#include <ee/core/LambdaAwaiter.hpp>
#include <ee/core/Task.hpp>
#include <ee/core/Utils.hpp>
#include <ee/nlohmann/json.hpp>

#include "ee/store/StoreConfigurationBuilder.hpp"
#include "ee/store/StoreProduct.hpp"
#include "ee/store/StoreProductCollection.hpp"
#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/StoreProductType.hpp"
#include "ee/store/private/StoreIAppleExtensions.hpp"
#include "ee/store/private/StoreIExtensionProvider.hpp"
#include "ee/store/private/StoreIGooglePlayStoreExtensions.hpp"
#include "ee/store/private/StoreIStoreController.hpp"
#include "ee/store/private/StoreIStoreListener.hpp"
#include "ee/store/private/StorePurchaseProcessingResult.hpp"
#include "ee/store/private/StoreSubscriptionManager.hpp"
#include "ee/store/private/StoreUnityPurchasing.hpp"

namespace ee {
namespace store {
using Self = Bridge;

class Self::Listener : public IStoreListener {
public:
    virtual void
    onInitializeFailed(InitializationFailureReason reason) override {
        onInitializedFailed_(reason);
    }

    virtual PurchaseProcessingResult
    processPurchase(const std::shared_ptr<Product>& product) override {
        return processPurchase_(product);
    }

    virtual void onPurchaseFailed(const std::shared_ptr<Product>& product,
                                  PurchaseFailureReason reason) override {
        onPurchaseFailed_(product, reason);
    }

    virtual void onInitialized(
        const std::shared_ptr<IStoreController>& controller,
        const std::shared_ptr<IExtensionProvider>& extensions) override {
        onInitialized_(controller, extensions);
    }

private:
    friend Bridge;

    std::function<void(InitializationFailureReason reason)>
        onInitializedFailed_;
    std::function<PurchaseProcessingResult(
        const std::shared_ptr<Product>& product)>
        processPurchase_;
    std::function<void(const std::shared_ptr<Product>& product,
                       PurchaseFailureReason reason)>
        onPurchaseFailed_;
    std::function<void(const std::shared_ptr<IStoreController>& controller,
                       const std::shared_ptr<IExtensionProvider>& extensions)>
        onInitialized_;
};

Self::Bridge(IMessageBridge& bridge, ILogger& logger,
             const Destroyer& destroyer)
    : bridge_(bridge)
    , logger_(logger)
    , destroyer_(destroyer) {
    initialized_ = false;
}

Self::~Bridge() = default;

void Self::destroy() {
    destroyer_();
}

Task<bool>
Self::initialize(const ConfigurationBuilder& builder,
                 const std::shared_ptr<ITransactionLog>& transactionLog) {
    if (initialized_) {
        co_return false;
    }
    if (initializationAwaiter_) {
        // Waiting.
    } else {
        initializationAwaiter_ = std::make_unique<LambdaAwaiter<bool>>(
            [this, builder, transactionLog](auto&& resolver) {
                initializationResolver_ = [this, resolver](auto&& result) {
                    initialized_ = result;
                    resolver(result);
                    initializationAwaiter_.reset();
                };
                initializeListener();
                UnityPurchasing::initialize(listener_, builder, transactionLog);
            });
    }
    auto result = co_await(*initializationAwaiter_);
    co_return result;
}

void Self::initializeListener() {
    listener_ = std::make_shared<Listener>();
    listener_->onInitializedFailed_ = [this](auto&& reason) {
        logger_.error("%s: onInitializedFailed: %d", __PRETTY_FUNCTION__,
                      static_cast<int>(reason));
        initializationResolver_(false);
    };
    listener_->processPurchase_ = [this](auto&& product) {
        if (purchaseResolver_) {
            purchaseResolver_(true);
        }
        return PurchaseProcessingResult::Complete;
    };
    listener_->onPurchaseFailed_ = [this](auto&& product, auto&& reason) {
        logger_.error("%s: onPurchaseFailed: %s %d", __PRETTY_FUNCTION__,
                      product->definition()->id().c_str(),
                      static_cast<int>(reason));
        if (purchaseResolver_) {
            purchaseResolver_(false);
        }
    };
    listener_->onInitialized_ = [this](auto&& controller, auto&& extensions) {
        controller_ = controller;
        extensions_ = extensions;
        initializationResolver_(true);
    };
}

std::shared_ptr<ProductCollection> Self::getProducts() const {
    return controller_ ? controller_->products() : nullptr;
}

std::shared_ptr<SubscriptionInfo>
Self::getSubscriptionInfo(const std::string& itemId) {
    if (not initialized_) {
        return nullptr;
    }
    auto&& product = controller_->products()->withId(itemId);
    if (product == nullptr) {
        // Invalid item ID.
        return nullptr;
    }
    if (not product->availableToPurchase()) {
        return nullptr;
    }
    if (product->receipt().empty()) {
        return nullptr;
    }
    if (product->definition()->type() != ProductType::Subscription) {
        return nullptr;
    }
    std::shared_ptr<SubscriptionManager> manager;
    if (auto extensions = extensions_->getExtension<IAppleExtensions>();
        extensions != nullptr) {
        auto&& dict = extensions->getIntroductoryPriceDictionary();
        auto&& introJson = dict[product->definition()->storeSpecificId()];
        manager = std::make_shared<SubscriptionManager>(product, introJson);
    } else {
        // Google Play.
        manager = std::make_shared<SubscriptionManager>(
            product->receipt(), product->definition()->storeSpecificId(), "");
    }
    return manager->getSubscriptionInfo();
}

Task<bool> Self::purchase(const std::string& itemId) {
    if (not initialized_) {
        co_return false;
    }
    if (purchaseAwaiter_) {
        co_return false;
    }
    purchaseAwaiter_ =
        std::make_unique<LambdaAwaiter<bool>>([this, itemId](auto&& resolver) {
            purchaseResolver_ = [this, resolver](auto&& result) {
                resolver(result);
                purchaseAwaiter_.reset();
            };
            controller_->initiatePurchase(itemId);
        });
    auto result = co_await(*purchaseAwaiter_);
    co_return result;
}

Task<bool> Self::restoreTransactions() {
    if (not initialized_) {
        co_return false;
    }
    if (restoreTransactionsAwaiter_) {
        // Waiting.
    } else {
        restoreTransactionsAwaiter_ =
            std::make_unique<LambdaAwaiter<bool>>([this](auto&& resolver) {
                if (auto extensions =
                        extensions_->getExtension<IGooglePlayStoreExtensions>();
                    extensions != nullptr) {
                    extensions->restoreTransactions(
                        [this, resolver](auto&& result) {
                            resolver(result);
                            restoreTransactionsAwaiter_.reset();
                        });
                    return;
                }
                if (auto extensions =
                        extensions_->getExtension<IAppleExtensions>();
                    extensions != nullptr) {
                    extensions->restoreTransactions(
                        [this, resolver](auto&& result) {
                            resolver(result);
                            restoreTransactionsAwaiter_.reset();
                        });
                    return;
                }
                assert(false);
                resolver(false);
                restoreTransactionsAwaiter_.reset();
            });
    }
    auto result = co_await(*restoreTransactionsAwaiter_);
    co_return result;
}
} // namespace store
} // namespace ee
