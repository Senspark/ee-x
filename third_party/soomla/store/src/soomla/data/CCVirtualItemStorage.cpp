/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "soomla/data/CCVirtualItemStorage.h"

#include <soomla/CCSoomlaUtils.h>
#include <soomla/data/CCKeyValueStorage.h>

namespace soomla {

USING_NS_CC;

#define TAG "SOOMLA VirtualItemStorage"

int CCVirtualItemStorage::getBalance(CCVirtualItem* item, CCError** error) {
    auto&& itemId = item->getId();
    auto&& key = keyBalance(itemId);
    const auto& val = CCKeyValueStorage::getInstance()->getValue(key);

    int balance = 0;
    if (val.length() > 0) {
        balance = std::stoi(val);
    }

    CCSoomlaUtils::logDebug(
        TAG,
        StringUtils::format("the balance for %s is %d", itemId.c_str(), balance)
            .c_str());

    return balance;
}

int CCVirtualItemStorage::setBalance(CCVirtualItem* item, int balance,
                                     bool notify, CCError** error) {
    int oldBalance = getBalance(item);
    if (oldBalance == balance) {
        return balance;
    }

    auto&& itemId = item->getId();

    auto&& balanceStr = StringUtils::format("%d", balance);
    auto&& key = keyBalance(itemId);

    CCKeyValueStorage::getInstance()->setValue(key, balanceStr);

    if (notify) {
        postBalanceChangeEvent(item, balance, 0);
    }

    return balance;
}

int CCVirtualItemStorage::add(CCVirtualItem* item, int amount, bool notify,
                              CCError** error) {
    auto&& itemId = item->getId();
    int balance = getBalance(item);
    if (balance < 0) { /* in case the user "adds" a negative value */
        balance = 0;
        amount = 0;
    }

    auto&& balanceStr = StringUtils::format("%d", balance + amount);
    auto&& key = keyBalance(itemId);

    CCKeyValueStorage::getInstance()->setValue(key, balanceStr);

    if (notify) {
        postBalanceChangeEvent(item, balance + amount, amount);
    }

    return balance + amount;
}

int CCVirtualItemStorage::remove(CCVirtualItem* item, int amount, bool notify,
                                 CCError** error) {
    auto&& itemId = item->getId();
    int balance = getBalance(item) - amount;
    if (balance < 0) {
        balance = 0;
        amount = 0;
    }

    auto&& balanceStr = StringUtils::format("%d", balance).c_str();
    auto&& key = keyBalance(itemId);

    CCKeyValueStorage::getInstance()->setValue(key, balanceStr);

    if (notify) {
        postBalanceChangeEvent(item, balance, -1 * amount);
    }

    return balance;
}
} // namespace soomla
