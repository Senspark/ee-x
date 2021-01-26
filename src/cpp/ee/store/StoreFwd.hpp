#ifndef EE_X_STORE_FWD_HPP
#define EE_X_STORE_FWD_HPP

#ifdef __cplusplus

#include <ee/core/CoreFwd.hpp>

namespace ee {
namespace store {
class IBridge;
class Bridge;

enum class AppleStoreProductType;
enum class AppStore;
enum class InitializationFailureReason;
enum class ProductType;
enum class PurchaseFailureReason;
enum class PurchaseProcessingResult;
enum class Result;
enum class StoreSpecificPurchaseErrorCode;
enum class SubscriptionPeriodUnit;

class IExtensionProvider;
class IInternalStoreListener;
class INativeStore;
class INativeAppleStore;
class INativeStoreProvider;
class IPurchasingBinder;
class IPurchasingModule;
class IStore;
class IStoreCallback;
class IStoreConfiguration;
class IStoreController;
class IStoreExtension;
class IStoreListener;
class ITransactionLog;
class IUnityCallback;

class AppleInAppPurchaseReceipt;
class AppleReceipt;
class ConfigurationBuilder;
class Product;
class ProductCollection;
class ProductDefinition;
class ProductDescription;
class ProductMetadata;
class PurchaseFailureDescription;
class PurchasingFactory;
class PurchasingManager;
class SessionTransactionLog;
class StandardPurchasingModule;
class SubscriptionManager;
class SubscriptionInfo;
class UnityPurchasing;
} // namespace store

using IStore = store::IBridge;
using StoreType = store::AppStore;
using StoreConfigurationBuilder = store::ConfigurationBuilder;
using StoreITransactionLog = store::ITransactionLog;
using StoreProduct = store::Product;
using StoreProductCollection = store::ProductCollection;
using StoreProductDefinition = store::ProductDefinition;
using StoreProductMetadata = store::ProductMetadata;
using StoreProductType = store::ProductType;
using StoreResult = store::Result;
using StoreSessionTransactionLog = store::SessionTransactionLog;
using StoreSubscriptionInfo = store::SubscriptionInfo;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_STORE_FWD_HPP
