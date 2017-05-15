//
//  PurchaseWithMarket.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "PurchaseWithMarket.hpp"

namespace ee {
namespace store {

void PurchaseWithMarket::buy(const std::string& payload, Error** error) {
    // FIXME.
}

bool PurchaseWithMarket::canAfford(Error** error) {
    // For market purchases, always assume it can be afforded.
    return true;
}
} // namespace store
} // namespace ee
