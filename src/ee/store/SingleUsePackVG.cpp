//
//  SingleUsePackVG.cpp
//  ee_x
//
//  Created by Zinge on 5/19/17.
//
//

#include "ee/store/SingleUsePackVG.hpp"
#include "ee/store/PurchaseType.hpp"

namespace ee {
namespace store {
int SingleUsePackVG::addBalance(int amount, bool notify) {
    auto goodItemId = getGoodItemId();
    //
    return 1;
}
} // namespace store
} // namespace ee
