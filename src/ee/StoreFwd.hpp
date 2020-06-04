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
} // namespace store

using StoreSkuType = store::SkuType;
using Store = store::Bridge;
using StorePurchase = store::Purchase;
using StorePurchaseHistoryRecord = store::PurchaseHistoryRecord;
using StoreSkuDetails = store::SkuDetails;
} // namespace ee

#endif // __cplusplus

#endif // EE_X_STORE_FWD_HPP
