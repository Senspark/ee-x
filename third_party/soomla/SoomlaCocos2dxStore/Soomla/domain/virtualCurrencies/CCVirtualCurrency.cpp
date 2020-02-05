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

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrency.h"

#include "Soomla/data/CCVirtualCurrencyStorage.h"

namespace soomla {

CCVirtualCurrency* CCVirtualCurrency::create(const std::string& name,
                                             const std::string& description,
                                             const std::string& itemId) {
    CCVirtualCurrency* ret = new CCVirtualCurrency();
    if (ret->init(itemId, name, description)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

const char* CCVirtualCurrency::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY;
}

int CCVirtualCurrency::give(int amount, bool notify, CCError** error) {
    return CCVirtualCurrencyStorage::getInstance()->add(this, amount, notify,
                                                        error);
}

int CCVirtualCurrency::take(int amount, bool notify, CCError** error) {
    return CCVirtualCurrencyStorage::getInstance()->remove(this, amount, notify,
                                                           error);
}

int CCVirtualCurrency::resetBalance(int balance, bool notify, CCError** error) {
    return CCVirtualCurrencyStorage::getInstance()->setBalance(this, balance,
                                                               notify, error);
}

int CCVirtualCurrency::getBalance(CCError** error) {
    return CCVirtualCurrencyStorage::getInstance()->getBalance(this, error);
}
} // namespace soomla
