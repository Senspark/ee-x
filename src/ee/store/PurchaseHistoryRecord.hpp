#ifndef EE_X_PURCHASE_HISTORY_RECORD_HPP
#define EE_X_PURCHASE_HISTORY_RECORD_HPP

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
struct PurchaseHistoryRecord {
    std::int64_t purchaseTime;
    std::string purchaseToken;
    std::string signature;
    std::string sku;
};
} // namespace store
} // namespace ee

#endif // EE_X_PURCHASE_HISTORY_RECORD_HPP