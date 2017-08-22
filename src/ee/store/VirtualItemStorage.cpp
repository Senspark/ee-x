//
//  VirtualItemStorage.cpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#include <functional>

#include "ee/store/VirtualItemStorage.hpp"
#include "ee/store/VirtualItem.hpp"
#include "ee/store/KeyValueStorage.hpp"
#include "ee/store/StoreLogger.hpp"
#include "ee/store/StoreUtils.hpp"

#define ENSURE_NON_NEGATIVE_BALANCE 0

namespace ee {
namespace store {
namespace {
constexpr auto Tag = "EEX VirtualItemStorage";
} // namespace

int VirtualItemStorage::getBalance(VirtualItem* item) {
    auto itemId = item->getItemId();
    auto key = keyBalance(itemId);
    auto val = KeyValueStorage::getInstance().getValue(key);

    auto logDebug =
        std::bind(&StoreLogger::logDebug, std::ref(StoreLogger::getInstance()),
                  Tag, std::placeholders::_1);

    if (val.empty()) {
        logDebug(format("The balance for %s has not set yet, return zero",
                        itemId.c_str()));
        return 0;
    }

    auto balance = std::stoi(val);
#if ENSURE_NON_NEGATIVE_BALANCE
    if (balance < 0) {
        logDebug(format("The balance for %s is negative (%d), return zero",
                        itemId.c_str(), balance));
        return 0;
    }
#endif // ENSURE_NON_NEGATIVE_BALANCE

    logDebug(format("The balance for %s is %d", itemId.c_str(), balance));
    return balance;
}

int VirtualItemStorage::setBalance(VirtualItem* item, int balance,
                                   bool notify) {
    auto oldBalance = getBalance(item);

#if ENSURE_NON_NEGATIVE_BALANCE
    if (oldBalance < 0) {
        oldBalance = 0;
    }

    if (balance < 0) {
        balance = 0;
    }
#endif // ENSURE_NON_NEGATIVE_BALANCE

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

#if ENSURE_NON_NEGATIVE_BALANCE
    if (balance < 0) {
        balance = 0;
    }
#endif // ENSURE_NON_NEGATIVE_BALANCE

    if (amount == 0) {
        return balance;
    }

#if ENSURE_NON_NEGATIVE_BALANCE
    if (balance + amount < 0) {
        amount = -balance;
    }
#endif // ENSURE_NON_NEGATIVE_BALANCE

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
