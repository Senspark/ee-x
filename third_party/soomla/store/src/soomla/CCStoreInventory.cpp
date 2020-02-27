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

#include "soomla/CCStoreInventory.h"

#include <soomla/CCSoomlaEventDispatcher.h>
#include <soomla/CCSoomlaUtils.h>

#include "soomla/data/CCStoreInfo.h"
#include "soomla/data/CCVirtualCurrencyStorage.h"
#include "soomla/data/CCVirtualGoodsStorage.h"
#include "soomla/domain/CCPurchasableVirtualItem.h"

namespace soomla {
#define TAG "SOOMLA StoreInventory"

USING_NS_CC;

static CCStoreInventory* s_SharedStoreInventory = nullptr;

CCStoreInventory* CCStoreInventory::sharedStoreInventory() {
    if (!s_SharedStoreInventory) {
        s_SharedStoreInventory = new CCStoreInventory();
        s_SharedStoreInventory->init();
    }

    return s_SharedStoreInventory;
}

CCStoreInventory::~CCStoreInventory() {}

bool CCStoreInventory::init() {
    // support reflection call to refreshLocalInventory
    CCSoomlaEventDispatcher::getInstance()->registerEventHandler(
        "Reflection::CCStoreInventory::refreshLocalInventory",
        [this](const cocos2d::ValueMap& parameters) {
            this->refreshLocalInventory();
        });
    return true;
}

bool CCStoreInventory::canAfford(const std::string& itemId, CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("Checking can afford: %s", itemId.c_str()));

    CCPurchasableVirtualItem* pvi = dynamic_cast<CCPurchasableVirtualItem*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error));
    return pvi->canAfford();
}

void CCStoreInventory::buyItem(const std::string& itemId, CCError** error) {
    buyItem(itemId, "", error);
}

void CCStoreInventory::buyItem(const std::string& itemId,
                               const std::string& payload, CCError** error) {
    CCSoomlaUtils::logDebug(TAG,
                            StringUtils::format("Buying: %s", itemId.c_str()));

    CCPurchasableVirtualItem* pvi = dynamic_cast<CCPurchasableVirtualItem*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error));
    if (pvi != nullptr) {
        pvi->buy(payload, error);
    }
}

int CCStoreInventory::getItemBalance(const std::string& itemId,
                                     CCError** error) {
    if (mLocalItemBalances.count(itemId)) {
        return mLocalItemBalances[itemId];
    }

    CCVirtualItem* item =
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error);
    if (item == nullptr) {
        return 0;
    }
    return item->getBalance(error);
}

void CCStoreInventory::giveItem(const std::string& itemId, int amount,
                                CCError** error) {
    CCSoomlaUtils::logDebug(TAG, StringUtils::format("Giving: %d pieces of: %s",
                                                     amount, itemId.c_str()));

    CCVirtualItem* item =
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error);
    if (item != nullptr) {
        item->give(amount, error);
    }
}

void CCStoreInventory::takeItem(const std::string& itemId, int amount,
                                CCError** error) {
    CCSoomlaUtils::logDebug(TAG, StringUtils::format("Taking: %d pieces of: %s",
                                                     amount, itemId.c_str()));

    CCVirtualItem* item =
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error);
    if (item != nullptr) {
        item->take(amount, error);
    }
}

void CCStoreInventory::equipVirtualGood(const std::string& itemId,
                                        CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("Equipping: %s", itemId.c_str()));

    CCEquippableVG* good = dynamic_cast<CCEquippableVG*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error));
    if (good != nullptr) {
        good->equip(error);
    }
}

void CCStoreInventory::unEquipVirtualGood(const std::string& itemId,
                                          CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("UnEquipping: %s", itemId.c_str()));

    CCEquippableVG* good = dynamic_cast<CCEquippableVG*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error));
    if (good != nullptr) {
        good->unequip(error);
    }
}

bool CCStoreInventory::isVirtualGoodEquipped(const std::string& itemId,
                                             CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("Checking if %s is equipped", itemId.c_str()));

    CCEquippableVG* good = dynamic_cast<CCEquippableVG*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId, error));
    if (good != nullptr) {
        return CCVirtualGoodsStorage::getInstance()->isEquipped(good, error);
    }

    return false;
}

int CCStoreInventory::getGoodUpgradeLevel(const std::string& goodItemId,
                                          CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG,
        StringUtils::format("Checking %s upgrade level", goodItemId.c_str()));

    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));
    if (good == nullptr) {
        CCSoomlaUtils::logError(
            TAG, "You tried to get the level of a non-existant virtual good.");
        return 0;
    }
    CCUpgradeVG* upgradeVG =
        CCVirtualGoodsStorage::getInstance()->getCurrentUpgrade(good, error);
    if (upgradeVG == nullptr) {
        return 0; // no upgrade
    }

    CCUpgradeVG* first =
        CCStoreInfo::sharedStoreInfo()->getFirstUpgradeForVirtualGood(
            goodItemId);
    int level = 1;
    while (first->getItemId().compare(upgradeVG->getItemId())) {
        first = dynamic_cast<CCUpgradeVG*>(
            CCStoreInfo::sharedStoreInfo()->getItemByItemId(
                first->getNextItemId(), error));
        level++;
    }

    return level;
}

std::string
CCStoreInventory::getGoodCurrentUpgrade(const std::string& goodItemId,
                                        CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG,
        StringUtils::format("Checking %s current upgrade", goodItemId.c_str()));

    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));
    if (good == nullptr) {
        return "";
    }

    CCUpgradeVG* upgradeVG =
        CCVirtualGoodsStorage::getInstance()->getCurrentUpgrade(good, error);
    if (upgradeVG == nullptr) {
        return "";
    }
    return upgradeVG->getItemId();
}

void CCStoreInventory::upgradeGood(const std::string& goodItemId,
                                   CCError** error) {
    CCSoomlaUtils::logDebug(TAG, StringUtils::format("Upgrading Good with: %s",
                                                     goodItemId.c_str()));
    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));
    if (good == nullptr) {
        return;
    }

    CCUpgradeVG* upgradeVG =
        CCVirtualGoodsStorage::getInstance()->getCurrentUpgrade(good, error);

    if (upgradeVG != nullptr) {
        const auto& nextItemId = upgradeVG->getNextItemId();
        if (CCSoomlaUtils::isEmpty(nextItemId)) {
            return;
        }
        CCUpgradeVG* vgu = dynamic_cast<CCUpgradeVG*>(
            CCStoreInfo::sharedStoreInfo()->getItemByItemId(nextItemId, error));
        if (vgu != nullptr) {
            vgu->buy("");
        }
    } else {
        CCUpgradeVG* first = dynamic_cast<CCUpgradeVG*>(
            CCStoreInfo::sharedStoreInfo()->getFirstUpgradeForVirtualGood(
                goodItemId));
        if (first != nullptr) {
            first->buy("");
        }
    }
}

void CCStoreInventory::removeGoodUpgrades(const std::string& goodItemId,
                                          CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("Removing Good Upgrades for: %s",
                                 goodItemId.c_str()));

    auto& upgrades =
        CCStoreInfo::sharedStoreInfo()->getUpgradesForVirtualGood(goodItemId);
    for (auto& upgrade : upgrades) {
        CCVirtualGoodsStorage::getInstance()->remove(upgrade, 1, true, error);
    }

    CCVirtualGood* good = dynamic_cast<CCVirtualGood*>(
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(goodItemId, error));
    CCVirtualGoodsStorage::getInstance()->removeUpgrades(good, error);
}

void CCStoreInventory::refreshLocalInventory() {
    mLocalItemBalances.clear();
    mLocalUpgrades.clear();
    mLocalEquippedGoods.clear();

    for (auto item : CCStoreInfo::sharedStoreInfo()->getCurrencies()) {
        mLocalItemBalances.emplace(
            item->getItemId(),
            CCVirtualCurrencyStorage::getInstance()->getBalance(item));
    }

    for (auto item : CCStoreInfo::sharedStoreInfo()->getGoods()) {
        int balance = CCVirtualGoodsStorage::getInstance()->getBalance(item);
        mLocalItemBalances.emplace(item->getItemId(), balance);

        CCUpgradeVG* upgrade =
            CCVirtualGoodsStorage::getInstance()->getCurrentUpgrade(item);
        if (upgrade != nullptr) {
            int upgradeLevel = getGoodUpgradeLevel(item->getItemId());
            CCLocalUpgrade* localUpgrade = CCLocalUpgrade::create();
            localUpgrade->setItemId(upgrade->getItemId());
            localUpgrade->setLevel(upgradeLevel);
            mLocalUpgrades.insert(item->getItemId(), localUpgrade);
        }

        CCEquippableVG* equippableVG = dynamic_cast<CCEquippableVG*>(item);
        if (equippableVG != nullptr) {
            if (CCVirtualGoodsStorage::getInstance()->isEquipped(
                    equippableVG)) {
                mLocalEquippedGoods.emplace(equippableVG->getId());
            }
        }
    }
}

void CCStoreInventory::refreshOnGoodUpgrade(CCVirtualGood* vg,
                                            CCUpgradeVG* uvg) {
    if (uvg == nullptr) {
        mLocalUpgrades.erase(vg->getItemId());
    } else {
        int upgradeLevel = getGoodUpgradeLevel(vg->getItemId());
        auto upgrade = mLocalUpgrades.at(vg->getItemId());
        if (upgrade != nullptr) {
            upgrade->setItemId(uvg->getItemId());
            upgrade->setLevel(upgradeLevel);
        } else {
            CCLocalUpgrade* localUpgrade = CCLocalUpgrade::create();
            localUpgrade->setItemId(uvg->getItemId());
            localUpgrade->setLevel(upgradeLevel);
            mLocalUpgrades.insert(vg->getItemId(), localUpgrade);
        }
    }
}

void CCStoreInventory::refreshOnGoodEquipped(CCEquippableVG* equippable) {
    mLocalEquippedGoods.insert(equippable->getId());
}

void CCStoreInventory::refreshOnGoodUnEquipped(CCEquippableVG* equippable) {
    mLocalEquippedGoods.erase(equippable->getId());
}

void CCStoreInventory::refreshOnCurrencyBalanceChanged(
    CCVirtualCurrency* virtualCurrency, int balance, int amountAdded) {
    updateLocalBalance(virtualCurrency->getItemId(), balance);
}

void CCStoreInventory::refreshOnGoodBalanceChanged(CCVirtualGood* good,
                                                   int balance,
                                                   int amountAdded) {
    updateLocalBalance(good->getItemId(), balance);
}

void CCStoreInventory::updateLocalBalance(const std::string& itemId,
                                          int balance) {
    if (mLocalItemBalances.count(itemId)) {
        mLocalItemBalances[itemId] = balance;
    } else {
        mLocalItemBalances.emplace(itemId, balance);
    }
}

CCStoreInventory::CCLocalUpgrade* CCStoreInventory::CCLocalUpgrade::create() {
    CCLocalUpgrade* ret = new CCLocalUpgrade();
    ret->autorelease();
    return ret;
}
} // namespace soomla
