#ifndef EE_X_STORE_FWD_HPP
#define EE_X_STORE_FWD_HPP

#ifdef __cplusplus

#include <ee/CoreFwd.hpp>

namespace ee {
namespace store {
class Bridge;

enum class InitializationFailureReason;
enum class ProductType;
enum class PurchaseFailureReason;
enum class PurchaseProcessingResult;
enum class StoreSpecificPurchaseErrorCode;
enum class AppStore;
enum class SubscriptionPeriodUnit;
enum class Result;
enum class AppleStoreProductType;

struct ProductDefinition;
struct ProductDescription;
struct ProductMetadata;
struct PurchaseFailureDescription;

class AppleReceipt;
class AppleInAppPurchaseReceipt;

class Product;
class ProductCollection;

class IPurchasingBinder;
class IPurchasingModule;

class IExtensionProvider;
class IInternalStoreListener;
class IStore;
class IStoreCallback;
class IStoreConfiguration;
class IStoreController;
class IStoreExtension;
class IStoreListener;
class ITransactionLog;
class IUnityCallback;

class INativeStore;
class INativeAppleStore;
class INativeStoreProvider;

class SubscriptionManager;
class SubscriptionInfo;

class ConfigurationBuilder;
class PurchasingFactory;
class PurchasingManager;
class StandardPurchasingModule;
class SessionTransactionLog;
class UnityPurchasing;
} // namespace store

using Store = store::Bridge;
using StoreConfigurationBuilder = store::ConfigurationBuilder;
using StoreITransactionLog = store::ITransactionLog;
using StoreProduct = store::Product;
using StoreProductCollection = store::ProductCollection;
using StoreProductDefinition = store::ProductDefinition;
using StoreProductMetadata = store::ProductMetadata;
using StoreProductType = store::ProductType;
using StoreSessionTransactionLog = store::SessionTransactionLog;
using StoreSubscriptionInfo = store::SubscriptionInfo;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_STORE_FWD_HPP
