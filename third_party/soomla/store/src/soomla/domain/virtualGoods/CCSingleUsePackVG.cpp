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

#include "soomla/domain/virtualGoods/CCSingleUsePackVG.h"

#include <soomla/CCJsonHelper.h>
#include <soomla/CCSoomlaUtils.h>

#include "soomla/data/CCStoreInfo.h"
#include "soomla/data/CCVirtualGoodsStorage.h"
#include "soomla/domain/virtualGoods/CCSingleUseVG.h"

namespace soomla {
USING_NS_CC;

#define TAG "SOOMLA SingleUsePackVG"

CCSingleUsePackVG* CCSingleUsePackVG::create(const std::string& goodItemId,
                                             std::int32_t goodAmount,
                                             const std::string& name,
                                             const std::string& description,
                                             const std::string& itemId,
                                             CCPurchaseType* purchaseType) {
    CCSingleUsePackVG* ret = new CCSingleUsePackVG();
    if (ret->init(goodItemId, goodAmount, name, description, itemId,
                  purchaseType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCSingleUsePackVG::init(const std::string& goodItemId,
                             std::int32_t goodAmount, const std::string& name,
                             const std::string& description,
                             const std::string& itemId,
                             CCPurchaseType* purchaseType) {
    bool res = CCVirtualGood::init(name, description, itemId, purchaseType);
    if (res) {
        setGoodItemId(goodItemId);
        setGoodAmount(goodAmount);
        return true;
    } else {
        return false;
    }
}

bool CCSingleUsePackVG::initWithValueMap(const cocos2d::ValueMap& map) {
    bool ret = CCVirtualGood::initWithValueMap(map);
    if (ret) {
        fillGoodItemIdFromValueMap(map);
        fillGoodAmountFromValueMap(map);
        return true;
    } else {
        return false;
    }
}

cocos2d::ValueMap CCSingleUsePackVG::toValueMap() {
    auto map = CCVirtualGood::toValueMap();

    putGoodItemIdToValueMap(map);
    putGoodAmountToValueMap(map);

    return map;
}

const char* CCSingleUsePackVG::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_SINGLE_USE_PACK_VG;
}

int CCSingleUsePackVG::give(int amount, bool notify, CCError** error) {
    auto&& goodItemId = getGoodItemId();
    CCSingleUseVG* good = dynamic_cast<CCSingleUseVG*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));

    if (good == nullptr) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format("SingleUseVG with itemId: %s doesn't "
                                     "exist! Can't give this pack.",
                                     goodItemId.c_str())
                     );
        return 0;
    }

    return CCVirtualGoodsStorage::getInstance()->add(
        good, getGoodAmount() * amount, notify, error);
}

int CCSingleUsePackVG::take(int amount, bool notify, CCError** error) {
    auto&& goodItemId = getGoodItemId();
    CCSingleUseVG* good = dynamic_cast<CCSingleUseVG*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));

    if (good == nullptr) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format("SingleUseVG with itemId: %s doesn't "
                                     "exist! Can't take this pack.",
                                     goodItemId.c_str())
                     );
        return 0;
    }

    return CCVirtualGoodsStorage::getInstance()->remove(
        good, getGoodAmount() * amount, notify, error);
}

int CCSingleUsePackVG::resetBalance(int balance, bool notify, CCError** error) {
    // Not supported for SingleUsePackVGs !
    CCSoomlaUtils::logError(
        TAG, "Someone tried to reset balance of GoodPack. That's not right.");
    return 0;
}

int CCSingleUsePackVG::getBalance(CCError** error) {
    // Not supported for SingleUsePackVGs !
    CCSoomlaUtils::logError(
        TAG, "Someone tried to check balance of GoodPack. That's not right.");
    return 0;
}

void CCSingleUsePackVG::fillGoodItemIdFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_VGP_GOOD_ITEM_ID)) {
        setGoodItemId(map.at(CCStoreConsts::JSON_VGP_GOOD_ITEM_ID).asString());
    } else {
        setGoodItemId("");
    }
}

void CCSingleUsePackVG::putGoodItemIdToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_VGP_GOOD_ITEM_ID] = getGoodItemId();
}

void CCSingleUsePackVG::fillGoodAmountFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_VGP_GOOD_AMOUNT)) {
        setGoodAmount(map.at(CCStoreConsts::JSON_VGP_GOOD_AMOUNT).asInt());
    } else {
        setGoodAmount(0);
    }
}

void CCSingleUsePackVG::putGoodAmountToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_VGP_GOOD_AMOUNT] = getGoodAmount();
}
} // namespace soomla
