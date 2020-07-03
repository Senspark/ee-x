#ifndef EE_X_STORE_FWD_HPP
#define EE_X_STORE_FWD_HPP

#ifdef __cplusplus

#include <ee/CoreFwd.hpp>

namespace ee {
namespace store {
enum class SkuType;

class Bridge;

struct Purchase;
struct PurchaseHistoryRecord;
struct SkuDetails;

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
class UnityPurchasing;
} // namespace store

using StoreSkuType = store::SkuType;
using Store = store::Bridge;
using StorePurchase = store::Purchase;
using StorePurchaseHistoryRecord = store::PurchaseHistoryRecord;
using StoreSkuDetails = store::SkuDetails;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_STORE_FWD_HPP
