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

#include "soomla/data/CCVirtualGoodsStorage.h"

#include <soomla/CCSoomlaUtils.h>
#include <soomla/data/CCKeyValueStorage.h>

#include "soomla/CCStoreEventDispatcher.h"
#include "soomla/NativeImpl/CCNativeVirtualGoodsStorage.h"
#include "soomla/data/CCStoreInfo.h"

namespace soomla {
#define TAG "SOOMLA VirtualGoodsStorage"

USING_NS_CC;

static CCVirtualGoodsStorage* s_SharedVirtualGoodsStorage = nullptr;

CCVirtualGoodsStorage* CCVirtualGoodsStorage::getInstance() {
    if (!s_SharedVirtualGoodsStorage) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        s_SharedVirtualGoodsStorage = new CCNativeVirtualGoodsStorage();
#else
        s_SharedVirtualGoodsStorage = new CCVirtualGoodsStorage();
#endif
        //            s_SharedVirtualGoodsStorage->retain();
    }
    return s_SharedVirtualGoodsStorage;
}

CCVirtualGoodsStorage::CCVirtualGoodsStorage() {}

CCVirtualGoodsStorage::~CCVirtualGoodsStorage() {}

void CCVirtualGoodsStorage::removeUpgrades(CCVirtualGood* good, bool notify,
                                           CCError** error) {
    auto&& itemId = good->getId();
    auto&& key = keyGoodUpgrade(itemId);

    CCKeyValueStorage::getInstance()->deleteKeyValue(key);

    if (notify) {
        CCStoreEventDispatcher::getInstance()->onGoodUpgrade(good, nullptr);
    }
}

void CCVirtualGoodsStorage::assignCurrentUpgrade(CCVirtualGood* good,
                                                 CCUpgradeVG* upgradeVG,
                                                 bool notify, CCError** error) {
    CCUpgradeVG* upgrade = getCurrentUpgrade(good, error);
    if ((upgrade != nullptr) &&
        (upgrade->getId().compare(upgradeVG->getId()) == 0)) {
        return;
    }

    auto&& itemId = good->getId();
    auto&& key = keyGoodUpgrade(itemId);
    auto&& upItemId = upgradeVG->getId();

    CCKeyValueStorage::getInstance()->setValue(key, upItemId);

    if (notify) {
        CCStoreEventDispatcher::getInstance()->onGoodUpgrade(good, upgradeVG);
    }
}

CCUpgradeVG* CCVirtualGoodsStorage::getCurrentUpgrade(CCVirtualGood* good,
                                                      CCError** error) {
    auto&& itemId = good->getId();
    auto&& key = keyGoodUpgrade(itemId);

    const auto& upItemId = CCKeyValueStorage::getInstance()->getValue(key);

    if (upItemId.length() == 0) {
        CCSoomlaUtils::logDebug(
            TAG, StringUtils::format("You tried to fetch the current upgrade "
                                     "of %s but there's not upgrade to it.",
                                     itemId.c_str())
                     .c_str());
        return nullptr;
    }

    CCVirtualItem* item =
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(upItemId, error);
    if (item == nullptr) {
        return nullptr;
    }

    CCUpgradeVG* upgrade = dynamic_cast<CCUpgradeVG*>(item);
    if (upgrade == nullptr) {
        CCSoomlaUtils::logDebug(
            TAG,
            "The current upgrade's itemId from the DB is not an UpgradeVG.");
        return nullptr;
    }

    return upgrade;
}

bool CCVirtualGoodsStorage::isEquipped(CCEquippableVG* good, CCError** error) {
    auto&& itemId = good->getId();
    auto&& key = keyGoodEquipped(itemId);
    const auto& val = CCKeyValueStorage::getInstance()->getValue(key);

    return (val.length() > 0);
}

void CCVirtualGoodsStorage::equip(CCEquippableVG* good, bool notify,
                                  CCError** error) {
    if (isEquipped(good)) {
        return;
    }

    equipPriv(good, true, notify);
}

void CCVirtualGoodsStorage::unequip(CCEquippableVG* good, bool notify,
                                    CCError** error) {
    if (!isEquipped(good)) {
        return;
    }

    equipPriv(good, false, notify);
}

void CCVirtualGoodsStorage::equipPriv(CCEquippableVG* good, bool equip,
                                      bool notify, CCError** error) {
    auto&& itemId = good->getId();
    auto&& key = keyGoodEquipped(itemId);

    if (equip) {
        CCKeyValueStorage::getInstance()->setValue(key, "yes");
        if (notify) {
            CCStoreEventDispatcher::getInstance()->onGoodEquipped(good);
        }
    } else {
        CCKeyValueStorage::getInstance()->deleteKeyValue(key);
        if (notify) {
            CCStoreEventDispatcher::getInstance()->onGoodUnEquipped(good);
        }
    }
}

std::string CCVirtualGoodsStorage::keyBalance(const std::string& itemId) const {
    return keyGoodBalance(itemId);
}

void CCVirtualGoodsStorage::postBalanceChangeEvent(CCVirtualItem* item,
                                                   int balance,
                                                   int amountAdded) {
    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(item);
    if (good == nullptr) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format("Trying to post good balance changed "
                                     "with a non VirtualGood item %s",
                                     item->getId().c_str())
                     .c_str());
        return;
    }

    CCStoreEventDispatcher::getInstance()->onGoodBalanceChanged(good, balance,
                                                                amountAdded);
}

std::string CCVirtualGoodsStorage::keyGoodBalance(const std::string& itemId) {
    return StringUtils::format("good.%s.balance", itemId.c_str());
}

std::string CCVirtualGoodsStorage::keyGoodEquipped(const std::string& itemId) {
    return StringUtils::format("good.%s.equipped", itemId.c_str());
}

std::string CCVirtualGoodsStorage::keyGoodUpgrade(const std::string& itemId) {
    return StringUtils::format("good.%s.currentUpgrade", itemId.c_str());
}
} // namespace soomla
