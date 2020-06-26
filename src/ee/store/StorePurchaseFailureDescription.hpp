//
//  StorePurchaseFailureDescription.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PURCHASE_FAILURE_DESCRIPTION_HPP
#define EE_X_STORE_PURCHASE_FAILURE_DESCRIPTION_HPP

#ifdef __cplusplus

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
struct PurchaseFailureDescription {
    std::string productId;
    PurchaseFailureReason reason;
    std::string message;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PURCHASE_FAILURE_DESCRIPTION_HPP */
