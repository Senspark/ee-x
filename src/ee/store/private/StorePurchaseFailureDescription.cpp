//
//  StorePurchaseFailureDescription.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/private/StorePurchaseFailureDescription.hpp"

namespace ee {
namespace store {
using Self = PurchaseFailureDescription;

Self::PurchaseFailureDescription(const std::string& productId,
                                 PurchaseFailureReason reason,
                                 const std::string& message)
    : productId_(productId)
    , reason_(reason)
    , message_(message) {}

const std::string& Self::productId() const {
    return productId_;
}

PurchaseFailureReason Self::reason() const {
    return reason_;
}

const std::string& Self::message() const {
    return message_;
}
} // namespace store
} // namespace ee
