//
//  PurchaseType.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "PurchaseType.hpp"

namespace ee {
namespace store {
PurchaseType::PurchaseType() = default;
PurchaseType::~PurchaseType() = default;

bool PurchaseType::init() {
    return true;
}

const std::string& PurchaseType::getAssociatedItemId() const {
    return associatedItemId_;
}

void PurchaseType::setAssociatedItemId(const std::string& itemId) {
    associatedItemId_ = itemId;
}
} // namespace store
} // namespace ee
