//
//  VirtualItemStorage.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include "ee/store/VirtualItemStorage.hpp"
#include "ee/store/VirtualItem.hpp"
#include "ee/store/KeyValueStorage.hpp"

namespace ee {
namespace store {
int VirtualItemStorage::getBalance(VirtualItem* item) {
    auto itemId = item->getItemId();
    auto key = keyBalance(itemId);
    auto val = KeyValueStorage::getInstance().getValue(key);
    auto balance = (val.empty() ? 0 : std::stoi(val));
    return balance;
}

int VirtualItemStorage::setBalance(VirtualItem* item, int balance,
                                   bool notify) {
    auto oldBalance = getBalance(item);
    if (oldBalance == balance) {
        return balance;
    }

    auto itemId = item->getItemId();
    auto key = keyBalance(itemId);
    auto val = std::to_string(balance);
    KeyValueStorage::getInstance().setValue(key, val);
    if (notify) {
        postBalanceChangeEvent(item, balance, balance - oldBalance);
    }
    return balance;
}

int VirtualItemStorage::addBalance(VirtualItem* item, int amount, bool notify) {
    auto balance = getBalance(item);
    if (amount == 0) {
        return balance;
    }

    auto itemId = item->getItemId();
    auto key = keyBalance(itemId);
    auto val = std::to_string(balance + amount);
    KeyValueStorage::getInstance().setValue(key, val);
    if (notify) {
        postBalanceChangeEvent(item, balance + amount, amount);
    }
    return balance + amount;
}
} // namespace store
} // namespace ee
