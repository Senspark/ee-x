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
class PurchaseFailureDescription {
public:
    explicit PurchaseFailureDescription(const std::string& productId,
                                        PurchaseFailureReason reason,
                                        const std::string& message);

    const std::string& productId() const;
    PurchaseFailureReason reason() const;
    const std::string& message() const;

private:
    std::string productId_;
    PurchaseFailureReason reason_;
    std::string message_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PURCHASE_FAILURE_DESCRIPTION_HPP */
