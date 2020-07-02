//
//  StorePurchaseFailureReason.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PURCHASE_FAILURE_REASON_HPP
#define EE_X_STORE_PURCHASE_FAILURE_REASON_HPP

#ifdef __cplusplus

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class PurchaseFailureReason {
    PurchasingUnavailable,
    ExistingPurchasePending,
    ProductUnavailable,
    SignatureInvalid,
    UserCancelled,
    PaymentDeclined,
    DuplicateTransaction,
    Unknown,
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PURCHASE_FAILURE_REASON_HPP */
