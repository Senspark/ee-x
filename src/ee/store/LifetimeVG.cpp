//
//  LifetimeVG.cpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#include "ee/store/LifetimeVG.hpp"
#include "ee/store/VirtualGoodsStorage.hpp"
#include "ee/store/PurchaseType.hpp"

namespace ee {
namespace store {
bool LifetimeVG::canBuy() {
    auto balance = VirtualGoodsStorage::getInstance().getBalance(this);
    return balance < 1;
}

int LifetimeVG::addBalance(int amount, bool notify) {
    return VirtualGoodsStorage::getInstance().addBalance(this, amount, notify);
}
} // namespace store
} // namespace ee
