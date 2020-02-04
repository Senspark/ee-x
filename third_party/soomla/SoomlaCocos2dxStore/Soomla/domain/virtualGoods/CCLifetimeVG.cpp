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

#include "Soomla/domain/virtualGoods/CCLifetimeVG.h"

#include <Soomla/CCSoomlaUtils.h>

#include "Soomla/data/CCVirtualGoodsStorage.h"

namespace soomla {

USING_NS_CC;

#define TAG "SOOMLA LifetimeVG"

CCLifetimeVG* CCLifetimeVG::create(const std::string& name,
                                   const std::string& description,
                                   const std::string& itemId,
                                   CCPurchaseType* purchaseType) {
    CCLifetimeVG* ret = new CCLifetimeVG();
    if (ret->init(name, description, itemId, purchaseType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool CCLifetimeVG::canBuy() {
    int balance = CCVirtualGoodsStorage::getInstance()->getBalance(this);

    return balance < 1;
}

int CCLifetimeVG::give(int amount, bool notify, CCError** error) {
    if (amount > 1) {
        CCSoomlaUtils::logDebug(TAG,
                                "You tried to give more than one LifetimeVG. \
                                    Will try to give one anyway.");
        amount = 1;
    }

    int balance = CCVirtualGoodsStorage::getInstance()->getBalance(this);

    if (balance < 1) {
        return CCVirtualGoodsStorage::getInstance()->add(this, amount, notify,
                                                         error);
    }
    return 1;
}

int CCLifetimeVG::take(int amount, bool notify, CCError** error) {
    if (amount > 1) {
        amount = 1;
    }

    int balance = CCVirtualGoodsStorage::getInstance()->getBalance(this);

    if (balance > 0) {
        return CCVirtualGoodsStorage::getInstance()->remove(this, amount,
                                                            notify, error);
    }
    return 0;
}

const char* CCLifetimeVG::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_LIFETIME_VG;
}
} // namespace soomla
