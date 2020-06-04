#ifndef EE_X_PURCHASE_HPP
#define EE_X_PURCHASE_HPP

#ifdef __cplusplus

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
struct Purchase {
    std::string packageName;
    int purchaseState;
    std::int64_t purchaseTime;
    std::string purchaseToken;
    std::string signature;
    std::string sku;
    bool isAcknowledged;
    bool isAutoRenewing;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif // EE_X_PURCHASE_HPP
