//
//  VirtualCurrencyStorage.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "ee/store/VirtualCurrencyStorage.hpp"
#include "ee/store/VirtualCurrency.hpp"

namespace ee {
namespace store {
std::string
VirtualCurrencyStorage::keyBalance(const std::string& itemId) const {
    return "currency." + itemId + ".balance";
}

void VirtualCurrencyStorage::postBalanceChangeEvent(VirtualItem* item,
                                                    int balance, int amount) {
    auto virtualCurrency = dynamic_cast<VirtualCurrency*>(item);
    if (virtualCurrency == nullptr) {
        return;
    }
    // FIXME.
}
} // namespace store
} // namespace ee
