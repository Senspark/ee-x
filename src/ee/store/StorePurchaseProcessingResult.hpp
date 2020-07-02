//
//  StorePurchaseProcessingResult.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PURCHASE_PROCESSING_RESULT_HPP
#define EE_X_STORE_PURCHASE_PROCESSING_RESULT_HPP

#ifdef __cplusplus

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class PurchaseProcessingResult {
    Complete,
    Pending,
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PURCHASE_PROCESSING_RESULT_HPP */
