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

#ifndef __CCStoreInfo_H_
#define __CCStoreInfo_H_

#include <cocos2d.h>

#include <soomla/CCError.h>

#include "soomla/domain/CCVirtualCategory.h"
#include "soomla/domain/CCVirtualItem.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrency.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"
#include "soomla/domain/virtualGoods/CCUpgradeVG.h"

#include "soomla/CCStoreAssets.h"

namespace soomla {
/**
 @class CCStoreInfo
 @brief An interface to the native `StoreInfo` class.

 This class holds the store's metadata including:
 virtual currencies,
 virtual currency packs,
 all virtual goods,
 virtual categories, and
 non-consumables
*/
class CCStoreInfo : public cocos2d::Ref {
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<CCVirtualCurrency*>, mCurrencies,
                              Currencies);
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<CCVirtualCurrencyPack*>,
                              mCurrencyPacks, CurrencyPacks);
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<CCVirtualGood*>, mGoods, Goods);
    CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vector<CCVirtualCategory*>, mCategories,
                              Categories);

public:
    /**
     This class is a singleton, use this function to access it.
    */
    static CCStoreInfo* sharedStoreInfo();

    /**
     Initializes `CCStoreInfo` on native side and allows using its
     functions.
     @param storeAssets An instance of your store's assets class.
     */
    static void createShared(CCStoreAssets* storeAssets);

    virtual bool init(CCStoreAssets* storeAssets);
    virtual bool initWithValueMap(cocos2d::ValueMap& map);

    CCStoreInfo() = default;

    /**
     Retrieves a single `CCVirtualItem` that resides in the metadata.
     @param itemId The item id of the item to be fetched.
     @param error A `CCError` for error checking.
     @return The virtual item with the given `itemId`.
     */
    CCVirtualItem* getItemByItemId(const std::string& itemId);
    CCVirtualItem* getItemByItemId(const std::string& itemId,
                                   CCError** error);

    /**
     Retrieves a single `CCPurchasableVirtualItem` that resides in the
     metadata.
     IMPORTANT: The retrieved `CCPurchasableVirtualItem` has a purchaseType
     of `CCPurchaseWithMarket`. This is why we fetch here with `productId`
     and not with `itemId` (`productId` is the id of the product in the App
     Store).
     @param productId The product id of the item to be fetched.
     @param error A `CCError` for error checking.
     @return The purchasable virtual item with the given `productId`.
    */
    CCPurchasableVirtualItem*
    getPurchasableItemWithProductId(const std::string& productId,
                                    CCError** error = nullptr);

    /**
     Retrieves the `CCVirtualCategory` that the `CCVirtualGood` with the
     given `goodItemId` belongs to.
     @param goodItemId The item id of the virtual good whose category is
            being retrieved.
     @param error A `CCError` for error checking.
     @return The `CCVirtualCategory` for the `CCVirtualGood` with the given
     `goodItemId`.
    */
    CCVirtualCategory* getCategoryForVirtualGood(const std::string& goodItemId,
                                                 CCError** error = nullptr);

    /**
     Retrieves the first `CCUpgradeVG` for the`CCVirtualGood` with the given
     `itemId`.
     @param goodItemId The `CCVirtualGood` whose upgrade is being retrieved.
     @return The first `CCUpgradeVG` for the virtual good with the given
            `itemId`.
    */
    CCUpgradeVG*
    getFirstUpgradeForVirtualGood(const std::string& goodItemId = nullptr);

    /**
     Retrieves the last `CCUpgradeVG` for the`CCVirtualGood` with the given
     `itemId`.
     @param goodItemId the `CCVirtualGood` we're searching the upgrade for.
     @return The last `CCUpgradeVG` for the virtual good with the given
            `ItemId`.
    */
    CCUpgradeVG*
    getLastUpgradeForVirtualGood(const std::string& goodItemId = nullptr);

    /**
     Retrieves all `CCUpgradeVG`s for the `CCVirtualGood` with the given
     `itemId`.
     @param goodItemId the `CCVirtualGood` we're searching the upgrades for.
     @return Array of all upgrades for the good with the given item id.
     */
    cocos2d::Vector<CCUpgradeVG*>&
    getUpgradesForVirtualGood(const std::string& goodItemId = nullptr);

    /**
     Replaces the given virtual item, and then saves the store's metadata.
     @param virtualItem The virtual item to save.
     @param saveToDB should the data persist to the local DB.
     */
    void saveItem(CCVirtualItem* virtualItem, bool saveToDB = true);

    /**
     Replaces the given virtual items, and then saves the store's metadata.
     @param virtualItems The virtual items to save.
     @param saveToDB should the data persist to the local DB.
     */
    void saveItems(const cocos2d::Vector<CCVirtualItem*>& virtualItems,
                   bool saveToDB = true);

    /**
     Saves the store's metadata in the database as JSON.
     */
    virtual void save();

    /**
     Converts the store info into a `CCDictionary`.
     @return `CCDictionary` representation of `CCStoreInfo`.
     */
    virtual cocos2d::ValueMap toValueMap();

protected:
    virtual void setStoreAssets(CCStoreAssets* storeAssets);
    virtual void initializeFromDB();
    void updateAggregatedLists();
    void replaceVirtualItem(CCVirtualItem* virtualItem);
    cocos2d::ValueMap storeAssetsToValueMap(CCStoreAssets* storeAssets);

    void
    setVirtualItems(const cocos2d::Map<std::string, CCVirtualItem*>& value);
    cocos2d::Map<std::string, CCVirtualItem*>& getVirtualItems();
    void setPurchasableItems(
        const cocos2d::Map<std::string, CCPurchasableVirtualItem*>& value);
    cocos2d::Map<std::string, CCPurchasableVirtualItem*>& getPurchasableItems();
    void setGoodsCategories(
        const cocos2d::Map<std::string, CCVirtualCategory*>& value);
    cocos2d::Map<std::string, CCVirtualCategory*>& getGoodsCategories();
    void setGoodsUpgrades(
        const std::unordered_map<std::string, cocos2d::Vector<CCUpgradeVG*>>&
            value);
    std::unordered_map<std::string, cocos2d::Vector<CCUpgradeVG*>>&
    getGoodsUpgrades();

private:
    bool validateStoreAssets(CCStoreAssets* storeAssets);
    bool hasMarketIdDuplicates(
        const cocos2d::Vector<CCPurchasableVirtualItem*>& assetsArray);

    cocos2d::Map<std::string, CCVirtualItem*> mVirtualItems;
    cocos2d::Map<std::string, CCPurchasableVirtualItem*> mPurchasableItems;
    cocos2d::Map<std::string, CCVirtualCategory*> mGoodsCategories;
    std::unordered_map<std::string, cocos2d::Vector<CCUpgradeVG*>>
        mGoodsUpgrades;
};
}; // namespace soomla

#endif //__CCStoreInfo_H_
