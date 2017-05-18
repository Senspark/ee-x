//
//  VirtualGoodsStorage.cpp
//  ee_x
//
//  Created by Zinge on 5/18/17.
//
//

#include "ee/store/VirtualGoodsStorage.hpp"
#include "ee/store/VirtualGood.hpp"

namespace ee {
namespace store {
std::string VirtualGoodsStorage::keyBalance(const std::string& itemId) const {
    return "good." + itemId + ".balance";
}

void VirtualGoodsStorage::postBalanceChangeEvent(VirtualItem* item, int balance,
                                                 int amount) {
    auto virtualGood = dynamic_cast<VirtualGood*>(item);
    if (virtualGood == nullptr) {
        return;
    }
    // FIXME.
}
} // namespace store
} // namespace ee
