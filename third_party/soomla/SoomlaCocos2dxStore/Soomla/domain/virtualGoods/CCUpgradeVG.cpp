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

#include "CCJsonHelper.h"
#include "CCSoomlaUtils.h"
#include "CCStoreInfo.h"
#include "CCUpgradeVG.h"
#include "CCVirtualGoodsStorage.h"

namespace soomla {
USING_NS_CC;

#define TAG "SOOMLA UpgradeVG"


CCUpgradeVG* CCUpgradeVG::create(const std::string& goodItemId,
                                 const std::string& prevItemId,
                                 const std::string& nextItemId,
                                 const std::string& name,
                                 const std::string& description,
                                 const std::string& itemId,
                                 soomla::CCPurchaseType* purchaseType) {
    CCUpgradeVG* ret = new CCUpgradeVG();
    if (ret->init(goodItemId, prevItemId, nextItemId, name, description, itemId,
                  purchaseType)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}


bool CCUpgradeVG::init(const std::string& goodItemId,
                       const std::string& prevItemId,
                       const std::string& nextItemId, const std::string& name,
                       const std::string& description,
                       const std::string& itemId,
                       CCPurchaseType* purchaseType) {
    bool res = CCLifetimeVG::init(name, description, itemId, purchaseType);
    if (res) {
        setGoodItemId(goodItemId);
        setPrevItemId(prevItemId);
        setNextItemId(nextItemId);
        return true;
    } else {
        return false;
    }
}

bool CCUpgradeVG::initWithValueMap(const cocos2d::ValueMap& map) {
    bool ret = CCLifetimeVG::initWithValueMap(map);
    if (ret) {
        fillGoodItemIdFromValueMap(map);
        fillPrevItemIdFromValueMap(map);
        fillNextItemIdFromValueMap(map);
        return true;
    } else {
        return false;
    }
}

ValueMap CCUpgradeVG::toValueMap() {
    ValueMap map = CCLifetimeVG::toValueMap();

    putGoodItemIdToValueMap(map);
    putPrevItemIdToValueMap(map);
    putNextItemIdToValueMap(map);

    return map;
}

bool CCUpgradeVG::canBuy() {
    CCError* error = nullptr;
    const char* goodItemId = getGoodItemId().c_str();
    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, &error));
    if ((error != nullptr) || (good == nullptr)) {
        CCSoomlaUtils::logError(TAG, StringUtils::format(
                                         "VirtualGood with itemId: %s doesn't "
                                         "exist! Returning false (can't buy).",
                                         goodItemId)
                                         .c_str());
        if (error != nullptr) {
            CCSoomlaUtils::logException(TAG, error);
        }
        return false;
    }

    CCUpgradeVG* upgradeVG =
        CCVirtualGoodsStorage::getInstance()->getCurrentUpgrade(good);

    return ((upgradeVG == nullptr && CCSoomlaUtils::isEmpty(getPrevItemId())) ||
            (upgradeVG != nullptr &&
             ((upgradeVG->getNextItemId().compare(getItemId()) == 0) ||
              (upgradeVG->getPrevItemId().compare(getItemId()) == 0)))) &&
           CCLifetimeVG::canBuy();
}

int CCUpgradeVG::give(int amount, bool notify, CCError** error) {
    const char* goodItemId = getGoodItemId().c_str();
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("Assigning %s to: %s",
                                        getName().c_str(), goodItemId)
                 .c_str());

    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));

    if (good == nullptr) {
        CCSoomlaUtils::logError(
            TAG,
            StringUtils::format(
                "VirtualGood with itemId: %s doesn't exist! Can't upgrade.",
                goodItemId)
                .c_str());
        return 0;
    }

    CCVirtualGoodsStorage::getInstance()->assignCurrentUpgrade(good, this,
                                                               notify, error);

    return CCLifetimeVG::give(amount, notify, error);
}

int CCUpgradeVG::take(int amount, bool notify, CCError** error) {
    const char* goodItemId = getGoodItemId().c_str();
    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));

    if (good == nullptr) {
        CCSoomlaUtils::logError(
            TAG,
            StringUtils::format(
                "VirtualGood with itemId: %s doesn't exist! Can't downgrade.",
                goodItemId)
                .c_str());
        return 0;
    }

    CCUpgradeVG* upgradeVG =
        CCVirtualGoodsStorage::getInstance()->getCurrentUpgrade(good, error);

    // Case: Upgrade is not assigned to this Virtual Good
    if (upgradeVG != this) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format(
                     "You can't take an upgrade that's not currently assigned. "
                     "The UpgradeVG %s is not assigned to the VirtualGood: %s",
                     getName().c_str(), good->getName().c_str())
                     .c_str());
        return 0;
    }

    if (!CCSoomlaUtils::isEmpty(getPrevItemId())) {
        const char* prevItemId = getPrevItemId().c_str();
        CCUpgradeVG* prevUpgradeVG = dynamic_cast<CCUpgradeVG*>(
            CCStoreInfo::sharedStoreInfo()->getItemByItemId(prevItemId, error));

        // Case: downgrade is not possible because previous upgrade does not
        // exist
        if (prevUpgradeVG == nullptr) {
            CCSoomlaUtils::logError(
                TAG,
                StringUtils::format("Previous UpgradeVG with itemId: %s "
                                           "doesn't exist! Can't downgrade.",
                                           prevItemId)
                    .c_str());
            return 0;
        }

        // Case: downgrade is successful!
        CCSoomlaUtils::logDebug(
            TAG, StringUtils::format("Downgrading %s to: %s",
                                            good->getName().c_str(),
                                            prevUpgradeVG->getName().c_str())
                     .c_str());

        CCVirtualGoodsStorage::getInstance()->assignCurrentUpgrade(
            good, prevUpgradeVG, notify, error);
    }

    // Case: first Upgrade in the series - so we downgrade to NO upgrade.
    else {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format("Downgrading %s to NO-UPGRADE",
                                            good->getName().c_str())
                     .c_str());
        CCVirtualGoodsStorage::getInstance()->removeUpgrades(good, notify,
                                                             error);
    }

    return CCLifetimeVG::take(amount, notify, error);
}

const char* CCUpgradeVG::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_UPGRADE_VG;
}

void CCUpgradeVG::fillGoodItemIdFromValueMap(const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_VGU_GOOD_ITEM_ID)) {
        setGoodItemId(map.at(CCStoreConsts::JSON_VGU_GOOD_ITEM_ID).asString());
    } else {
        setGoodItemId("");
    }
}

void CCUpgradeVG::putGoodItemIdToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_VGU_GOOD_ITEM_ID] = getGoodItemId();
}

void CCUpgradeVG::fillPrevItemIdFromValueMap(const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_VGU_PREV_ITEM_ID)) {
        setPrevItemId(map.at(CCStoreConsts::JSON_VGU_PREV_ITEM_ID).asString());
    } else {
        setPrevItemId("");
    }
}

void CCUpgradeVG::putPrevItemIdToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_VGU_PREV_ITEM_ID] = getPrevItemId();
}

void CCUpgradeVG::fillNextItemIdFromValueMap(const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_VGU_NEXT_ITEM_ID)) {
        setNextItemId(map.at(CCStoreConsts::JSON_VGU_NEXT_ITEM_ID).asString());
    } else {
        setNextItemId("");
    }
}

void CCUpgradeVG::putNextItemIdToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_VGU_NEXT_ITEM_ID] = getNextItemId();
}
} // namespace soomla
