//
//  StoreInitializationFailureReason.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_INITIALIZATION_FAILURE_REASON_HPP
#define EE_X_STORE_INITIALIZATION_FAILURE_REASON_HPP

#ifdef __cplusplus

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class InitializationFailureReason {
    PurchasingUnavailable,
    NoProductsAvailable,
    AppNotKnown,
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_INITIALIZATION_FAILURE_REASON_HPP */
