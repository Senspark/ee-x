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

#include "soomla/data/CCStoreInfo.h"

#include <set>

#include <soomla/CCDomainFactory.h>
#include <soomla/CCDomainHelper.h>
#include <soomla/CCJsonHelper.h>
#include <soomla/CCSoomlaEventDispatcher.h>
#include <soomla/CCSoomlaUtils.h>
#include <soomla/data/CCKeyValueStorage.h>

#include "soomla/NativeImpl/CCNativeStoreInfo.h"
#include "soomla/PurchaseTypes/CCPurchaseWithMarket.h"
#include "soomla/domain/CCMarketItem.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrency.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"
#include "soomla/domain/virtualGoods/CCEquippableVG.h"
#include "soomla/domain/virtualGoods/CCSingleUsePackVG.h"
#include "soomla/domain/virtualGoods/CCSingleUseVG.h"

namespace soomla {

#define TAG "SOOMLA StoreInfo"

#define KEY_META_STORE_INFO "meta.storeinfo"

USING_NS_CC;

static CCStoreInfo* s_SharedStoreInfo = nullptr;

CCStoreInfo* CCStoreInfo::sharedStoreInfo() {
    return s_SharedStoreInfo;
}

void CCStoreInfo::createShared(CCStoreAssets* storeAssets) {
    CCStoreInfo* ret = nullptr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ret = new CCNativeStoreInfo();
#else
    ret = new CCStoreInfo();
#endif
    if (ret->init(storeAssets)) {
        s_SharedStoreInfo = ret;
    } else {
        delete ret;
    }
}

bool CCStoreInfo::hasMarketIdDuplicates(
    const cocos2d::Vector<CCPurchasableVirtualItem*>& assetsArray) {
    std::set<std::string>* marketIds = new std::set<std::string>();
    bool result = true;
    for (auto pvi : assetsArray) {
        if (typeid(*(pvi->getPurchaseType())) == typeid(CCPurchaseWithMarket)) {
            std::string currentMarketId =
                dynamic_cast<CCPurchaseWithMarket*>(pvi->getPurchaseType())
                    ->getMarketItem()
                    ->getProductId();
            if (marketIds->find(currentMarketId) != marketIds->end()) {
                result = false;
                break;
            } else {
                marketIds->insert(currentMarketId);
            }
        }
    }
    delete marketIds;
    return result;
}

bool CCStoreInfo::validateStoreAssets(CCStoreAssets* storeAssets) {
    if (storeAssets == nullptr) {
        CCSoomlaUtils::logError(TAG, "The given store assets can't be null!");
        return false;
    }

    Vector<CCPurchasableVirtualItem*> goods;
    for (auto good : getGoods()) {
        goods.pushBack(good);
    }

    Vector<CCPurchasableVirtualItem*> packs;
    for (auto pack : getCurrencyPacks()) {
        packs.pushBack(pack);
    }
    if (!CCStoreInfo::hasMarketIdDuplicates(goods) ||
        !CCStoreInfo::hasMarketIdDuplicates(packs)) {
        CCSoomlaUtils::logError(
            TAG,
            "The given store assets has duplicates at marketItem productId!");
        return false;
    }
    return true;
}

bool CCStoreInfo::init(CCStoreAssets* storeAssets) {
    CCSoomlaUtils::logDebug(TAG, "Setting store assets in SoomlaInfo");

    if (!validateStoreAssets(storeAssets)) {
        return false;
    }

    // support reflection call to initializeFromDB
    CCSoomlaEventDispatcher::getInstance()->registerEventHandler(
        "Reflection::CCStoreInfo::initializeFromDB",
        [this](const cocos2d::ValueMap& parameters) {
            this->initializeFromDB();
        });

    setStoreAssets(storeAssets);

    // At this point we have StoreInfo JSON saved at the local key-value
    // storage. We can just continue by initializing from DB.

    initializeFromDB();

    return true;
}

CCVirtualItem* CCStoreInfo::getItemByItemId(const std::string& itemId) {
    return getItemByItemId(itemId, nullptr);
}

CCVirtualItem* CCStoreInfo::getItemByItemId(const std::string& itemId,
                                            CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("Trying to fetch an item with itemId: %s",
                                 itemId.c_str()));

    auto iter = getVirtualItems().find(itemId);
    if (iter != getVirtualItems().cend()) {
        return iter->second;
    }

    CCError::tryFillError(
        error, Value(StringUtils::format(
                   "Virtual item was not found when searching with itemId=%s",
                   itemId.c_str())));
    return nullptr;
}

CCPurchasableVirtualItem*
CCStoreInfo::getPurchasableItemWithProductId(const std::string& productId,
                                             CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "Trying to fetch a purchasable item with productId: %s",
                 productId.c_str()));

    auto iter = getPurchasableItems().find(productId);
    if (iter != getPurchasableItems().cend()) {
        return iter->second;
    }

    CCError::tryFillError(
        error,
        Value(StringUtils::format(
            "Virtual item was not found when searching with productId=%s",
            productId.c_str())));
    return nullptr;
}

CCVirtualCategory*
CCStoreInfo::getCategoryForVirtualGood(const std::string& goodItemId,
                                       CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "Trying to fetch a category for a good with itemId: %s",
                 goodItemId.c_str()));

    auto iter = getGoodsCategories().find(goodItemId);
    if (iter != getGoodsCategories().cend()) {
        return iter->second;
    }

    CCError::tryFillError(error, Value(StringUtils::format(
                                     "Virtual item was not found when "
                                     "searching with goodItemId of category=%s",
                                     goodItemId.c_str())));
    return nullptr;
}

CCUpgradeVG*
CCStoreInfo::getFirstUpgradeForVirtualGood(const std::string& goodItemId) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "Trying to fetch first upgrade of a good with itemId: %s",
                 goodItemId.c_str()));

    if (getGoodsUpgrades().find(goodItemId) != getGoodsUpgrades().end()) {
        auto&& upgrades = getGoodsUpgrades().at(goodItemId);
        for (auto upgrade : upgrades) {
            if (CCSoomlaUtils::isEmpty(upgrade->getPrevItemId())) {
                return upgrade;
            }
        }
    }
    return nullptr;
}

CCUpgradeVG*
CCStoreInfo::getLastUpgradeForVirtualGood(const std::string& goodItemId) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "Trying to fetch last upgrade of a good with itemId: %s",
                 goodItemId.c_str()));

    if (getGoodsUpgrades().find(goodItemId) != getGoodsUpgrades().end()) {
        auto&& upgrades = getGoodsUpgrades().at(goodItemId);
        for (auto upgrade : upgrades) {
            if (CCSoomlaUtils::isEmpty(upgrade->getNextItemId())) {
                return upgrade;
            }
        }
    }

    return nullptr;
}

cocos2d::Vector<CCUpgradeVG*>&
CCStoreInfo::getUpgradesForVirtualGood(const std::string& goodItemId) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "Trying to fetch upgrades of a good with itemId: %s",
                 goodItemId.c_str()));

    return getGoodsUpgrades().at(goodItemId);
}

void CCStoreInfo::saveItem(CCVirtualItem* virtualItem, bool saveToDB) {
    replaceVirtualItem(virtualItem);

    if (saveToDB) {
        save();
    }
}

void CCStoreInfo::saveItems(const cocos2d::Vector<CCVirtualItem*>& virtualItems,
                            bool saveToDB) {
    if (virtualItems.size() == 0) {
        return;
    }

    for (auto virtualItem : virtualItems) {
        replaceVirtualItem(virtualItem);
    }

    if (saveToDB) {
        save();
    }
}

void CCStoreInfo::save() {
    auto&& storeDict = toValueMap();
    const char* jsonString =
        json_dumps(CCJsonHelper::getJsonFromValue(Value(storeDict)),
                   JSON_COMPACT | JSON_ENSURE_ASCII);
    CCSoomlaUtils::logDebug(
        TAG,
        StringUtils::format("saving StoreInfo to DB. json is: %s", jsonString));
    CCKeyValueStorage::getInstance()->setValue(KEY_META_STORE_INFO, jsonString);
}

ValueMap
CCStoreInfo::storeAssetsToValueMap(soomla::CCStoreAssets* storeAssets) {
    ValueVector currencies;
    for (auto currency : storeAssets->getCurrencies()) {
        currencies.push_back(Value(currency->toValueMap()));
    }

    ValueVector packs;
    for (auto pack : storeAssets->getCurrencyPacks()) {
        packs.push_back(Value(pack->toValueMap()));
    }

    ValueVector suGoods;
    ValueVector ltGoods;
    ValueVector eqGoods;
    ValueVector upGoods;
    ValueVector paGoods;

    for (auto good : storeAssets->getGoods()) {
        if (dynamic_cast<CCSingleUseVG*>(good)) {
            suGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCEquippableVG*>(good)) {
            eqGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCUpgradeVG*>(good)) {
            upGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCLifetimeVG*>(good)) {
            ltGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCSingleUsePackVG*>(good)) {
            paGoods.push_back(Value(good->toValueMap()));
        }
    }

    ValueMap mapGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_SU] = suGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_LT] = ltGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_EQ] = eqGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_UP] = upGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_PA] = paGoods;

    ValueVector categories;
    for (auto category : storeAssets->getCategories()) {
        categories.push_back(Value(category->toValueMap()));
    }

    ValueMap storeAssetsMap;
    storeAssetsMap[CCStoreConsts::JSON_STORE_CATEGORIES] = categories;
    storeAssetsMap[CCStoreConsts::JSON_STORE_CURRENCIES] = currencies;
    storeAssetsMap[CCStoreConsts::JSON_STORE_CURRENCY_PACKS] = packs;
    storeAssetsMap[CCStoreConsts::JSON_STORE_GOODS] = mapGoods;

    return storeAssetsMap;
}

void CCStoreInfo::setStoreAssets(CCStoreAssets* storeAssets) {
    auto&& storeMap = storeAssetsToValueMap(storeAssets);
    const char* jsonString =
        json_dumps(CCJsonHelper::getJsonFromValue(Value(storeMap)),
                   JSON_COMPACT | JSON_ENSURE_ASCII);
    CCKeyValueStorage::getInstance()->setValue(KEY_META_STORE_INFO, jsonString);
}

void CCStoreInfo::initializeFromDB() {
    const auto& val =
        CCKeyValueStorage::getInstance()->getValue(KEY_META_STORE_INFO);

    if (val.length() == 0) {
        CCSoomlaUtils::logError(
            TAG,
            "store json is not in DB. Make sure you initialized SoomlaStore "
            "with your Store assets. The App will shut down now.");
        CC_ASSERT(false);
    }

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("the metadata-economy json (from DB) is %s",
                                 val.c_str()));

    json_error_t error;
    json_t* storeJson = json_loads(val.c_str(), 0, &error);
    if (!storeJson) {
        CCSoomlaUtils::logError(TAG, "Unable to parse metadata-economy JSON");
        CC_ASSERT(false);
    }

    ValueMap storeMap = CCJsonHelper::getValueFromJson(storeJson).asValueMap();
    initWithValueMap(storeMap);
}

void CCStoreInfo::updateAggregatedLists() {
    for (auto vi : getCurrencies()) {
        getVirtualItems().insert(vi->getItemId(), vi);
    }

    for (auto vi : getCurrencyPacks()) {
        getVirtualItems().insert(vi->getItemId(), vi);
        CCPurchaseType* purchaseType = vi->getPurchaseType();
        CCPurchaseWithMarket* purchaseWithMarket =
            dynamic_cast<CCPurchaseWithMarket*>(purchaseType);
        if (purchaseWithMarket != nullptr) {
            getPurchasableItems().insert(
                purchaseWithMarket->getMarketItem()->getProductId(), vi);
        }
    }

    for (auto vi : getGoods()) {
        getVirtualItems().insert(vi->getItemId(), vi);

        CCUpgradeVG* upgradeVG = dynamic_cast<CCUpgradeVG*>(vi);
        if (upgradeVG != nullptr) {
            auto&& upgrades = getGoodsUpgrades()[upgradeVG->getGoodItemId()];
            upgrades.pushBack(upgradeVG);
        }
        CCPurchaseType* purchaseType = vi->getPurchaseType();
        CCPurchaseWithMarket* purchaseWithMarket =
            dynamic_cast<CCPurchaseWithMarket*>(purchaseType);
        if (purchaseWithMarket != nullptr) {
            getPurchasableItems().insert(
                purchaseWithMarket->getMarketItem()->getProductId(), vi);
        }
    }

    for (auto category : getCategories()) {
        for (auto goodItemId : category->getGoodItemIds()) {
            getGoodsCategories().insert(goodItemId, category);
        }
    }
}

void CCStoreInfo::replaceVirtualItem(CCVirtualItem* virtualItem) {
    getVirtualItems().insert(virtualItem->getItemId(), virtualItem);

    CCVirtualCurrency* currency = dynamic_cast<CCVirtualCurrency*>(virtualItem);
    if (currency != nullptr) {
        for (auto currentCurrency : getCurrencies()) {
            if (not currentCurrency->getItemId().compare(
                    currency->getItemId())) {
                mCurrencies.eraseObject(currentCurrency);
                break;
            }
        }
        mCurrencies.pushBack(currency);
    }

    CCVirtualCurrencyPack* vcp =
        dynamic_cast<CCVirtualCurrencyPack*>(virtualItem);
    if (vcp != nullptr) {
        CCPurchaseWithMarket* purchaseWithMarket =
            dynamic_cast<CCPurchaseWithMarket*>(vcp->getPurchaseType());
        if (purchaseWithMarket != nullptr) {
            getPurchasableItems().insert(
                purchaseWithMarket->getMarketItem()->getProductId(), vcp);
        }

        for (auto currentCurrencyPack : getCurrencyPacks()) {
            if (not currentCurrencyPack->getItemId().compare(
                    vcp->getItemId())) {
                mCurrencyPacks.eraseObject(currentCurrencyPack);
                break;
            }
        }
        mCurrencyPacks.pushBack(vcp);
    }

    CCVirtualGood* vg = dynamic_cast<CCVirtualGood*>(virtualItem);
    if (vg != nullptr) {
        CCUpgradeVG* upgradeVG = dynamic_cast<CCUpgradeVG*>(vg);
        if (upgradeVG != nullptr) {
            if (getGoodsUpgrades().find(upgradeVG->getGoodItemId()) ==
                std::end(getGoodsUpgrades())) {
                getGoodsUpgrades().emplace(upgradeVG->getGoodItemId(),
                                           Vector<CCUpgradeVG*>());
            }

            auto& upgrades = getGoodsUpgrades().at(upgradeVG->getGoodItemId());
            upgrades.pushBack(upgradeVG);
        }

        CCPurchaseWithMarket* purchaseWithMarket =
            dynamic_cast<CCPurchaseWithMarket*>(vg->getPurchaseType());
        if (purchaseWithMarket != nullptr) {
            getPurchasableItems().insert(
                purchaseWithMarket->getMarketItem()->getProductId(), vg);
        }

        for (auto currentVG : getGoods()) {
            if (not currentVG->getItemId().compare(vg->getItemId())) {
                mGoods.eraseObject(currentVG);
                break;
            }
        }

        mGoods.pushBack(vg);
    }
}

bool CCStoreInfo::initWithValueMap(cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_STORE_CURRENCIES)) {
        auto& vvCurrencies =
            map.at(CCStoreConsts::JSON_STORE_CURRENCIES).asValueVector();
        auto&& domainsOfCurrency =
            CCDomainHelper::getInstance()->getDomainsFromValueVector(
                vvCurrencies);
        for (auto domain : domainsOfCurrency) {
            auto currency = dynamic_cast<CCVirtualCurrency*>(domain);
            CC_ASSERT(currency);
            mCurrencies.pushBack(currency);
        }
    }

    if (map.count(CCStoreConsts::JSON_STORE_CURRENCY_PACKS)) {
        auto& vvCurrencyPacks =
            map.at(CCStoreConsts::JSON_STORE_CURRENCY_PACKS).asValueVector();
        auto&& domainsOfCurrencyPacks =
            CCDomainHelper::getInstance()->getDomainsFromValueVector(
                vvCurrencyPacks);
        for (auto domain : domainsOfCurrencyPacks) {
            auto currencyPack = dynamic_cast<CCVirtualCurrencyPack*>(domain);
            CC_ASSERT(currencyPack);
            mCurrencyPacks.pushBack(currencyPack);
        }
    }

    if (map.count(CCStoreConsts::JSON_STORE_GOODS)) {
        auto& mapGoods = map.at(CCStoreConsts::JSON_STORE_GOODS).asValueMap();

        if (mapGoods.count(CCStoreConsts::JSON_STORE_GOODS_SU)) {
            auto& vvSingleUseGoods =
                mapGoods.at(CCStoreConsts::JSON_STORE_GOODS_SU).asValueVector();
            auto&& domainsOfSUGoods =
                CCDomainHelper::getInstance()->getDomainsFromValueVector(
                    vvSingleUseGoods);

            for (auto domain : domainsOfSUGoods) {
                auto suGood = dynamic_cast<CCSingleUseVG*>(domain);
                CC_ASSERT(suGood);
                mGoods.pushBack(suGood);
            }
        }

        if (mapGoods.count(CCStoreConsts::JSON_STORE_GOODS_LT)) {
            auto& vvLifetimeGoods =
                mapGoods.at(CCStoreConsts::JSON_STORE_GOODS_LT).asValueVector();
            auto&& domainsOfLTGoods =
                CCDomainHelper::getInstance()->getDomainsFromValueVector(
                    vvLifetimeGoods);

            for (auto domain : domainsOfLTGoods) {
                auto ltGood = dynamic_cast<CCLifetimeVG*>(domain);
                CC_ASSERT(ltGood);
                mGoods.pushBack(ltGood);
            }
        }

        if (mapGoods.count(CCStoreConsts::JSON_STORE_GOODS_EQ)) {
            auto& vvEquippableGoods =
                mapGoods.at(CCStoreConsts::JSON_STORE_GOODS_EQ).asValueVector();
            auto&& domainsOfEquippableGoods =
                CCDomainHelper::getInstance()->getDomainsFromValueVector(
                    vvEquippableGoods);

            for (auto domain : domainsOfEquippableGoods) {
                auto equippableGood = dynamic_cast<CCEquippableVG*>(domain);
                CC_ASSERT(equippableGood);
                mGoods.pushBack(equippableGood);
            }
        }

        if (mapGoods.count(CCStoreConsts::JSON_STORE_GOODS_PA)) {
            auto& vvSUPGoods =
                mapGoods.at(CCStoreConsts::JSON_STORE_GOODS_PA).asValueVector();
            auto&& domainsOfSUPGoods =
                CCDomainHelper::getInstance()->getDomainsFromValueVector(
                    vvSUPGoods);

            for (auto domain : domainsOfSUPGoods) {
                auto supGood = dynamic_cast<CCSingleUsePackVG*>(domain);
                CC_ASSERT(supGood);
                mGoods.pushBack(supGood);
            }
        }

        if (mapGoods.count(CCStoreConsts::JSON_STORE_GOODS_UP)) {
            auto& vvUpgradeGoods =
                mapGoods.at(CCStoreConsts::JSON_STORE_GOODS_UP).asValueVector();
            auto&& domainsOfUpgradeGoods =
                CCDomainHelper::getInstance()->getDomainsFromValueVector(
                    vvUpgradeGoods);

            for (auto domain : domainsOfUpgradeGoods) {
                auto upgradeGood = dynamic_cast<CCUpgradeVG*>(domain);
                CC_ASSERT(upgradeGood);
                mGoods.pushBack(upgradeGood);
            }
        }
    }

    if (map.count(CCStoreConsts::JSON_STORE_CATEGORIES)) {
        auto& vvCategories =
            map.at(CCStoreConsts::JSON_STORE_CATEGORIES).asValueVector();
        auto&& domainsOfCategories =
            CCDomainHelper::getInstance()->getDomainsFromValueVector(
                vvCategories);
        for (auto domain : domainsOfCategories) {
            auto category = dynamic_cast<CCVirtualCategory*>(domain);
            CC_ASSERT(category);
            mCategories.pushBack(category);
        }
    }

    updateAggregatedLists();

    return true;
}

ValueMap CCStoreInfo::toValueMap() {
    ValueVector vvCurrencies;
    for (auto currency : getCurrencies()) {
        vvCurrencies.push_back(Value(currency->toValueMap()));
    }

    ValueVector vvPacks;
    for (auto pack : getCurrencyPacks()) {
        vvPacks.push_back(Value(pack->toValueMap()));
    }

    ValueVector suGoods;
    ValueVector ltGoods;
    ValueVector eqGoods;
    ValueVector upGoods;
    ValueVector paGoods;

    for (auto good : getGoods()) {
        if (dynamic_cast<CCSingleUseVG*>(good)) {
            suGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCEquippableVG*>(good)) {
            eqGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCUpgradeVG*>(good)) {
            upGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCLifetimeVG*>(good)) {
            ltGoods.push_back(Value(good->toValueMap()));
        } else if (dynamic_cast<CCSingleUsePackVG*>(good)) {
            paGoods.push_back(Value(good->toValueMap()));
        }
    }

    ValueMap mapGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_SU] = suGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_LT] = ltGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_EQ] = eqGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_UP] = upGoods;
    mapGoods[CCStoreConsts::JSON_STORE_GOODS_PA] = paGoods;

    ValueVector vvCategories;
    for (auto category : getCategories()) {
        vvCategories.push_back(Value(category->toValueMap()));
    }

    ValueMap storeAssetsObj;
    storeAssetsObj[CCStoreConsts::JSON_STORE_CATEGORIES] = vvCategories;
    storeAssetsObj[CCStoreConsts::JSON_STORE_CURRENCIES] = vvCurrencies;
    storeAssetsObj[CCStoreConsts::JSON_STORE_CURRENCY_PACKS] = vvPacks;
    storeAssetsObj[CCStoreConsts::JSON_STORE_GOODS] = mapGoods;

    return storeAssetsObj;
}

void CCStoreInfo::setVirtualItems(
    const cocos2d::Map<std::string, CCVirtualItem*>& value) {
    mVirtualItems = value;
}

cocos2d::Map<std::string, CCVirtualItem*>& CCStoreInfo::getVirtualItems() {
    return mVirtualItems;
}

void CCStoreInfo::setPurchasableItems(
    const cocos2d::Map<std::string, CCPurchasableVirtualItem*>& value) {
    mPurchasableItems = value;
}

cocos2d::Map<std::string, CCPurchasableVirtualItem*>&
CCStoreInfo::getPurchasableItems() {
    return mPurchasableItems;
}

void CCStoreInfo::setGoodsCategories(
    const cocos2d::Map<std::string, CCVirtualCategory*>& value) {
    mGoodsCategories = value;
}

cocos2d::Map<std::string, CCVirtualCategory*>&
CCStoreInfo::getGoodsCategories() {
    return mGoodsCategories;
}

void CCStoreInfo::setGoodsUpgrades(
    const std::unordered_map<std::string, cocos2d::Vector<CCUpgradeVG*>>&
        value) {
    mGoodsUpgrades = value;
}
std::unordered_map<std::string, cocos2d::Vector<CCUpgradeVG*>>&
CCStoreInfo::getGoodsUpgrades() {
    return mGoodsUpgrades;
}
} // namespace soomla
