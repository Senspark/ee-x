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

struct ProductDefinition;
struct ProductDescription;
struct ProductMetadata;
struct PurchaseFailureDescription;

class Product;
class ProductCollection;

class IStore;
class IStoreCallback;
class IStoreController;
class IStoreListener;
class ITransactionLog;

class PurchasingManager;
} // namespace store

using StoreSkuType = store::SkuType;
using Store = store::Bridge;
using StorePurchase = store::Purchase;
using StorePurchaseHistoryRecord = store::PurchaseHistoryRecord;
using StoreSkuDetails = store::SkuDetails;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_STORE_FWD_HPP
