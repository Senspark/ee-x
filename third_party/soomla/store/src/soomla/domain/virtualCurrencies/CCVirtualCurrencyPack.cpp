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


#include "soomla/domain/virtualCurrencies/CCVirtualCurrency.h"

#include <soomla/CCJsonHelper.h>
#include <soomla/CCSoomlaUtils.h>

#include "soomla/data/CCStoreInfo.h"
#include "soomla/data/CCVirtualCurrencyStorage.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"

namespace soomla {
USING_NS_CC;

#define TAG "SOOMLA VirtualCurrencyPack"

CCVirtualCurrencyPack* CCVirtualCurrencyPack::create(
    const std::string& name, const std::string& description,
    const std::string& itemId, std::int32_t currencyAmount,
    const std::string& currencyItemId, CCPurchaseType* purchaseType) {
    CCVirtualCurrencyPack* ret = new CCVirtualCurrencyPack();
    if (ret->init(name, description, itemId, currencyAmount, currencyItemId,
                  purchaseType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCVirtualCurrencyPack::init(const std::string& name,
                                 const std::string& description,
                                 const std::string& itemId,
                                 std::int32_t currencyAmount,
                                 const std::string& currencyItemId,
                                 CCPurchaseType* purchaseType) {
    bool res =
        CCPurchasableVirtualItem::init(name, description, itemId, purchaseType);
    if (res) {
        setCurrencyAmount(currencyAmount);
        setCurrencyItemId(currencyItemId);
        return true;
    } else {
        return false;
    }
}

bool CCVirtualCurrencyPack::initWithValueMap(const cocos2d::ValueMap& map) {
    bool ret = CCPurchasableVirtualItem::initWithValueMap(map);
    if (ret) {
        fillCurrencyAmountFromValueMap(map);
        fillCurrencyItemIdFromValueMap(map);
        return true;
    } else {
        return false;
    }
}

cocos2d::ValueMap CCVirtualCurrencyPack::toValueMap() {
    ValueMap map = CCPurchasableVirtualItem::toValueMap();
    putCurrencyAmountToValueMap(map);
    putCurrencyItemIdToValueMap(map);

    return map;
}

const char* CCVirtualCurrencyPack::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK;
}

int CCVirtualCurrencyPack::give(int amount, bool notify, CCError** error) {
    const char* currencyId = getCurrencyItemId().c_str();
    CCVirtualCurrency* currency = dynamic_cast<CCVirtualCurrency*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(currencyId, error));

    if (currency == nullptr) {
        CCSoomlaUtils::logError(TAG, StringUtils::format(
                                         "VirtualCurrency with itemId: %s "
                                         "doesn't exist! Can't give this pack.",
                                         currencyId)
                                         .c_str());
        return 0;
    }

    return CCVirtualCurrencyStorage::getInstance()->add(
        currency, getCurrencyAmount() * amount, notify, error);
}

int CCVirtualCurrencyPack::take(int amount, bool notify, CCError** error) {
    const char* currencyId = getCurrencyItemId().c_str();
    CCVirtualCurrency* currency = dynamic_cast<CCVirtualCurrency*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(currencyId, error));

    if (currency == nullptr) {
        CCSoomlaUtils::logError(TAG, StringUtils::format(
                                         "VirtualCurrency with itemId: %s "
                                         "doesn't exist! Can't take this pack.",
                                         currencyId)
                                         .c_str());
        return 0;
    }

    return CCVirtualCurrencyStorage::getInstance()->remove(
        currency, getCurrencyAmount() * amount, notify, error);
}

int CCVirtualCurrencyPack::resetBalance(int balance, bool notify,
                                        CCError** error) {
    // Not supported for VirtualCurrencyPacks !
    CCSoomlaUtils::logError(
        TAG,
        "Someone tried to reset balance of CurrencyPack. That's not right.");
    return 0;
}

int CCVirtualCurrencyPack::getBalance(CCError** error) {
    // Not supported for VirtualCurrencyPacks !
    CCSoomlaUtils::logError(
        TAG,
        "Someone tried to check balance of CurrencyPack. That's not right.");
    return 0;
}

void CCVirtualCurrencyPack::fillCurrencyAmountFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_CURRENCY_PACK_CURRENCY_AMOUNT)) {
        setCurrencyAmount(
            map.at(CCStoreConsts::JSON_CURRENCY_PACK_CURRENCY_AMOUNT).asInt());
    }
}

void CCVirtualCurrencyPack::putCurrencyAmountToValueMap(
    cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_CURRENCY_PACK_CURRENCY_AMOUNT] =
        getCurrencyAmount();
}

void CCVirtualCurrencyPack::fillCurrencyItemIdFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_CURRENCY_PACK_CURRENCY_ITEM_ID)) {
        setCurrencyItemId(
            map.at(CCStoreConsts::JSON_CURRENCY_PACK_CURRENCY_ITEM_ID)
                .asString());
    }
}

void CCVirtualCurrencyPack::putCurrencyItemIdToValueMap(
    cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_CURRENCY_PACK_CURRENCY_ITEM_ID] =
        getCurrencyItemId();
}
} // namespace soomla
