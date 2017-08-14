//
//  SingleUseVG.cpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#include "ee/store/SingleUseVG.hpp"
#include "ee/store/VirtualGoodsStorage.hpp"
#include "ee/store/PurchaseType.hpp"

namespace ee {
namespace store {
bool SingleUseVG::canBuy() {
    return true;
}

int SingleUseVG::addBalance(int amount, bool notify) {
    return VirtualGoodsStorage::getInstance().addBalance(this, amount, notify);
}
} // namespace store
} // namespace ee
