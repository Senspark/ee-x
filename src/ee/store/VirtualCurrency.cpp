//
//  VirtualCurrency.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "ee/store/VirtualCurrency.hpp"
#include "ee/store/VirtualCurrencyStorage.hpp"

namespace ee {
namespace store {
int VirtualCurrency::getBalance() {
    return VirtualCurrencyStorage::getInstance().getBalance(this);
}

int VirtualCurrency::setBalance(int balance, bool notify) {
    return VirtualCurrencyStorage::getInstance().setBalance(this, balance,
                                                            notify);
}

int VirtualCurrency::addBalance(int amount, bool notify) {
    return VirtualCurrencyStorage::getInstance().addBalance(this, amount,
                                                            notify);
}
} // namespace store
} // namespace ee
