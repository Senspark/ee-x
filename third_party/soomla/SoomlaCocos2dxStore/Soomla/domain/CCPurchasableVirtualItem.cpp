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

#include "CCPurchasableVirtualItem.h"

#include "../PurchaseTypes/CCPurchaseWithMarket.h"
#include "../PurchaseTypes/CCPurchaseWithVirtualItem.h"
#include "CCJsonHelper.h"

namespace soomla {

using namespace cocos2d;

bool CCPurchasableVirtualItem::init(const std::string& name,
                                    const std::string& description,
                                    const std::string& itemId,
                                    soomla::CCPurchaseType* purchaseType) {
    bool bRet = CCVirtualItem::init(itemId, name, description);
    if (bRet) {
        setPurchaseType(purchaseType);
        purchaseType->setAssociatedItemId(getItemId());
    }
    return bRet;
}

bool CCPurchasableVirtualItem::initWithValueMap(const cocos2d::ValueMap& map) {
    bool bRet = CCVirtualItem::initWithValueMap(map);
    if (bRet) {
        fillPurchaseTypeFromValueMap(map);
        return true;
    } else {
        return false;
    }
}

CCPurchasableVirtualItem::~CCPurchasableVirtualItem() {
    CC_SAFE_RELEASE(mPurchaseType);
}

ValueMap CCPurchasableVirtualItem::toValueMap() {
    auto&& map = CCVirtualItem::toValueMap();
    putPurchaseTypeToValueMap(map);
    return map;
}

bool CCPurchasableVirtualItem::canAfford(CCError** error) {
    return getPurchaseType()->canAfford(error);
}

void CCPurchasableVirtualItem::buy(const std::string& payload, CCError** error) {
    if (!canBuy()) {
        return;
    }

    getPurchaseType()->buy(payload, error);
}

void CCPurchasableVirtualItem::fillPurchaseTypeFromValueMap(
    const cocos2d::ValueMap& dict) {
    CC_ASSERT(dict.count(CCStoreConsts::JSON_PURCHASABLE_ITEM));
    auto& purchasableDict =
        dict.at(CCStoreConsts::JSON_PURCHASABLE_ITEM).asValueMap();
    CCAssert(purchasableDict.count(CCStoreConsts::JSON_PURCHASE_TYPE),
             "invalid object type in dictionary");
    auto&& purchaseTypeStr =
        purchasableDict.at(CCStoreConsts::JSON_PURCHASE_TYPE).asString();

    if (purchaseTypeStr.compare(CCStoreConsts::JSON_PURCHASE_TYPE_MARKET) ==
        0) {
        CC_ASSERT(purchasableDict.count(CCStoreConsts::JSON_PURCHASE_MARKET_ITEM));
        auto& marketItemDict =
            purchasableDict.at(CCStoreConsts::JSON_PURCHASE_MARKET_ITEM).asValueMap();

        setPurchaseType(CCPurchaseWithMarket::createWithMarketItem(
            CCMarketItem::createWithValueMap(marketItemDict)));
    } else if (purchaseTypeStr.compare(CCStoreConsts::JSON_PURCHASE_TYPE_VI) ==
               0) {
        CC_ASSERT(
            purchasableDict.count(CCStoreConsts::JSON_PURCHASE_VI_ITEM_ID));
        auto&& itemId =
            purchasableDict.at(CCStoreConsts::JSON_PURCHASE_VI_ITEM_ID)
                .asString();
        CC_ASSERT(
            purchasableDict.count(CCStoreConsts::JSON_PURCHASE_VI_AMOUNT));
        auto amount =
            purchasableDict.at(CCStoreConsts::JSON_PURCHASE_VI_AMOUNT).asInt();

        setPurchaseType(CCPurchaseWithVirtualItem::create(itemId, amount));
    } else {
        CC_ASSERT(false);
        cocos2d::log(
            "Couldn't determine what type of class is the given purchaseType.");
    }

    CCPurchaseType* purchaseType = getPurchaseType();
    if (purchaseType != NULL) {
        purchaseType->setAssociatedItemId(getItemId());
    }
}

void CCPurchasableVirtualItem::putPurchaseTypeToValueMap(
    cocos2d::ValueMap& dict) {
    ValueMap purchasableObj;

    if (dynamic_cast<CCPurchaseWithMarket*>(mPurchaseType)) {
        purchasableObj[CCStoreConsts::JSON_PURCHASE_TYPE] =
            CCStoreConsts::JSON_PURCHASE_TYPE_MARKET;

        CCPurchaseWithMarket* purchaseWithMarket =
            (CCPurchaseWithMarket*)mPurchaseType;
        CCMarketItem* mi = purchaseWithMarket->getMarketItem();
        purchasableObj[CCStoreConsts::JSON_PURCHASE_MARKET_ITEM] =
            mi->toValueMap();
    } else if (dynamic_cast<CCPurchaseWithVirtualItem*>(mPurchaseType)) {
        CCPurchaseWithVirtualItem* purchaseWithVirtualItem =
            (CCPurchaseWithVirtualItem*)mPurchaseType;
        purchasableObj[CCStoreConsts::JSON_PURCHASE_TYPE] =
            CCStoreConsts::JSON_PURCHASE_TYPE_VI;
        purchasableObj[CCStoreConsts::JSON_PURCHASE_VI_ITEM_ID] =
            purchaseWithVirtualItem->getItemId();
        purchasableObj[CCStoreConsts::JSON_PURCHASE_VI_AMOUNT] =
            purchaseWithVirtualItem->getAmount();
    } else {
        CC_ASSERT(false);
    }
    
    dict[CCStoreConsts::JSON_PURCHASABLE_ITEM] = purchasableObj;
}

} // namespace soomla
