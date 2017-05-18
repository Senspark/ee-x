//
//  PurchasableVirtualItem.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "ee/store/PurchasableVirtualItem.hpp"
#include "ee/store/PurchaseType.hpp"

namespace ee {
namespace store {
const std::unique_ptr<PurchaseType>&
PurchasableVirtualItem::getPurchaseType() const {
    return purchaseType_;
}

bool PurchasableVirtualItem::canAfford() {
    return getPurchaseType()->canAfford();
}

void PurchasableVirtualItem::buy(const std::string& payload) {
    if (not canBuy()) {
        return;
    }
    getPurchaseType()->buy(payload);
}
} // namespace store
} // namespace ee
