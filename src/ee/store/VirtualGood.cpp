//
//  VirtualGood.cpp
//  ee_x
//
//  Created by Zinge on 5/18/17.
//
//

#include "ee/store/VirtualGood.hpp"
#include "ee/store/VirtualGoodsStorage.hpp"
#include "ee/store/PurchaseType.hpp"

namespace ee {
namespace store {
int VirtualGood::getBalance() {
    return VirtualGoodsStorage::getInstance().getBalance(this);
}

int VirtualGood::setBalance(int balance, bool notify) {
    return VirtualGoodsStorage::getInstance().setBalance(this, balance, notify);
}
} // namespace store
} // namespace ee
