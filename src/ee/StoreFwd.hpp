#ifndef EE_X_STORE_FWD_HPP
#define EE_X_STORE_FWD_HPP

#include <ee/CoreFwd.hpp>

namespace ee {
namespace store {
class Bridge;

struct Purchase;
struct PurchaseHistoryRecord;
struct SkuDetails;
} // namespace store

using Store = store::Bridge;
} // namespace ee

#endif // EE_X_STORE_FWD_HPP
