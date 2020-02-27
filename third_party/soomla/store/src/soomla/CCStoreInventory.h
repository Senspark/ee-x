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

#ifndef __CCStoreInventory_H_
#define __CCStoreInventory_H_

#include <set>

#include <cocos2d.h>

#include <soomla/CCError.h>

#include "soomla/domain/virtualCurrencies/CCVirtualCurrency.h"
#include "soomla/domain/virtualGoods/CCEquippableVG.h"
#include "soomla/domain/virtualGoods/CCUpgradeVG.h"
#include "soomla/domain/virtualGoods/CCVirtualGood.h"

namespace soomla {
/** @class CCStoreInventory
    @brief An interface to the native StoreInventory class.

    An interface to the native StoreInventory class, use this class to
    access the native StoreInventory functionality.
 */
class CCStoreInventory : public cocos2d::Ref {
public:
    /**
     This class is singleton, use this function to access it.
     */
    static CCStoreInventory* sharedStoreInventory();

    CCStoreInventory() = default;

    virtual bool init();

    virtual ~CCStoreInventory(void);

    /**
     Checks if there is enough funds to afford `itemId`.
     @param itemId id of item to be checked
     @param error A `CCError` for error checking.
     @return True if there are enough funds to afford the virtual item
     with the given item id
     */
    bool canAfford(const std::string& itemId, CCError** error = nullptr);

    /**
     Buys the item with the given `itemId`.
     @param itemId The id of the item to be purchased.
     @param error A `CCError` for error checking.
     */
    void buyItem(const std::string& itemId, CCError** error = nullptr);

    /**
     Buys the item with the given `itemId`.
     @param itemId The id of the item to be purchased.
     @param payload Payload.
     @param error A `CCError` for error checking.
     */
    void buyItem(const std::string& itemId, const std::string& payload,
                 CCError** error = nullptr);

    /**
     Retrieves the balance of the virtual item with the given `itemId`.
     @param itemId The id of the virtual item to be fetched - must be of
     `CCVirtualCurrency`, `CCSingleUseVG`, `CCLifetimeVG`, `CCEquippableVG`.
     @param error A `CCError` for error checking.
     @return The balance of the virtual item with the given `itemId`.
    */
    int getItemBalance(const std::string& itemId, CCError** error = nullptr);

    /**
     Gives your user the given amount of the virtual item with the given
     `itemId`.
     For example, when your user plays your game for the first time you GIVE
     him/her 1000 gems.
     NOTE: This action is different than `buy` -
     You use `give` to give your user something for free.
     You use `buy` to give your user something and get something in return.
     @param amount The amount of the item to be given.
     @param itemId The id of the virtual item to be given.
     @param error A `CCError`for error checking.
     */
    void giveItem(const std::string& itemId, int amount, CCError** error = nullptr);

    /**
     Takes from your user the given amount of the virtual item with the
     given `itemId`.
     For example, when you want to downgrade a virtual good, you take the
     upgrade.
     @param amount The amount of the item to be given.
     @param itemId The id of the virtual item to be taken.
     @param error A `CCError` for error checking.
     */
    void takeItem(const std::string& itemId, int amount, CCError** error = nullptr);

    /**
     Equips the virtual good with the given `goodItemId`.
     Equipping means that the user decides to currently use a specific
     virtual good. For more details and examples see `CCEquippableVG`.
     @param itemId The id of the virtual good to be equipped. Id MUST
            be of a `CCEquippableVG`.
     @param error A `CCError` for error checking.
    */
    void equipVirtualGood(const std::string& itemId, CCError** error = nullptr);

    /**
     Unequips the virtual good with the given `goodItemId`. Unequipping
     means that the user decides to stop using the virtual good he/she is
     currently using. For more details and examples see `CCEquippableVG`.
     @param itemId The id of the virtual good to be unequipped. Id MUST
            be of a `CCEquippableVG`.
     @param error A `CCError` for error checking.
     */
    void unEquipVirtualGood(const std::string& itemId, CCError** error = nullptr);

    /**
     Checks if the virtual good with the given `goodItemId` is equipped
     (currently being used).
     @param itemId The id of the virtual good to check on. Id MUST be of
            an `EquippableVG`.
     @param error A `CCError` for error checking.
     @return True if the virtual good with the given id is equipped, false
            otherwise.
     */
    bool isVirtualGoodEquipped(const std::string& itemId, CCError** error = nullptr);

    /**
     Retrieves the upgrade level of the virtual good with the given
     `goodItemId`.
     For Example:
     Let's say there's a strength attribute to one of the characters in your
     game and you provide your users with the ability to upgrade that
     strength on a scale of 1-3.
     This is what you've created:
     1. `CCSingleUseVG` for "strength"
     2. `CCUpgradeVG` for strength 'level 1'
     3. `CCUpgradeVG` for strength 'level 2'
     4. `CCUpgradeVG` for strength 'level 3'
     In the example, this function will retrieve the upgrade level for
     "strength" (1, 2, or 3).
     @param goodItemId The id of the virtual good whose upgrade level we
            want to know. The `goodItemId` can be of any `CCVirtualGood`.
     @param error A `CCError` for error checking.
     @return The upgrade level.
     */
    int getGoodUpgradeLevel(const std::string& goodItemId, CCError** error = nullptr);

    /**
     Retrieves the item id of the current upgrade of the virtual good with
     the given `goodItemId`.
     @param goodItemId Id of the virtual good whose upgrade id we want to
            know. The `goodItemId` can be of any `CCVirtualGood`.
     @param error A `CCError` for error checking.
     @return The upgrade id if exists, or empty string otherwise.
     */
    std::string getGoodCurrentUpgrade(const std::string& goodItemId,
                                      CCError** error = nullptr);

    /**
     Upgrades the virtual good with the given `goodItemId` by doing the
     following:
     1. Checks if the good is currently upgraded or if this is the first
     time being upgraded.
     2. If the good is currently upgraded, upgrades to the next upgrade in
     the series, or in other words, buys the next upgrade. In case there
     are no more upgrades available (meaning the current upgrade is the last
     available) the function returns.
     3. If the good has never been upgraded before, the function upgrades it
     to the first available upgrade, or in other words, buy()s the first
     upgrade in the series.
     @param goodItemId The id of the virtual good to be upgraded. The
            `upgradeItemId` can be of any `CCUpgradeVG`.
     @param error A `CCError` for error checking.
     */
    void upgradeGood(const std::string& goodItemId, CCError** error = nullptr);

    /**
     Removes all upgrades from the virtual good with the given `goodItemId`.
     @param goodItemId The id of the virtual good we want to remove all
            upgrades from. The `upgradeItemId` can be of any `UpgradeVG`.
     @param error A `CCError` for error checking.
     */
    void removeGoodUpgrades(const std::string& goodItemId, CCError** error = nullptr);

    /**
     This function refreshes a local set of objects that will hold your user's
     balances in memory for quick and more efficient fetching for your game UI.
     This way, we save many JNI or static calls to native platforms.

     NOTE: You don't need to call this function as it's automatically called
     when the game initializes. NOTE: This is less useful when you work in
     editor.
     */
    void refreshLocalInventory();

    /** Internal methods **/

    void refreshOnGoodUpgrade(CCVirtualGood* vg, CCUpgradeVG* uvg);
    void refreshOnGoodEquipped(CCEquippableVG* equippable);
    void refreshOnGoodUnEquipped(CCEquippableVG* equippable);
    void refreshOnCurrencyBalanceChanged(CCVirtualCurrency* virtualCurrency,
                                         int balance, int amountAdded);
    void refreshOnGoodBalanceChanged(CCVirtualGood* good, int balance,
                                     int amountAdded);
    void updateLocalBalance(const std::string& itemId, int balance);

private:
    class CCLocalUpgrade : public cocos2d::Ref {
    public:
        static CCLocalUpgrade* create();
        CCLocalUpgrade()
            : mLevel(0)
            , mItemId("") {}

        CC_SYNTHESIZE(std::int32_t, mLevel, Level);
        CC_SYNTHESIZE_PASS_BY_REF(std::string, mItemId, ItemId);
    };

    std::unordered_map<std::string, std::int32_t> mLocalItemBalances;
    cocos2d::Map<std::string, CCLocalUpgrade*> mLocalUpgrades;
    std::set<std::string> mLocalEquippedGoods;
};
}; // namespace soomla

#endif //__CCStoreInventory_H_
