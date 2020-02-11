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

#ifndef __CCVirtualCurrencyPack_H_
#define __CCVirtualCurrencyPack_H_

#include <soomla/CCSoomlaMacros.h>
#include "soomla/CCStoreConsts.h"

#include "soomla/domain/CCPurchasableVirtualItem.h"

namespace soomla {
/**
 @class CCVirtualCurrencyPack
 @brief A virtual currency pack.

 Every game has its virtual currencies. Here you represent a pack of a
 specific `CCVirtualCurrency`. Real Game Example: If virtual currency in
 your game is a 'Coin', you will sell packs of 'Coins' (e.g. "10 Coins Set"
 or "Super Saver Pack").

 NOTE: In case you want this item to be available for purchase in the native
 store (with real money) you'll need to define the item in the App Store,
 Google Play, etc..

 Inheritance: CCVirtualCurrencyPack > CCPurchasableVirtualItem >
 CCVirtualItem
 */
class CCVirtualCurrencyPack : public CCPurchasableVirtualItem {
public:
    CCVirtualCurrencyPack()
        : CCPurchasableVirtualItem()
        , mCurrencyAmount(0)
        , mCurrencyItemId(""){};

    /**
     Creates a `CCVirtualCurrencyPack`.
     @param name see parent.
     @param description see parent.
     @param itemId see parent.
     @param currencyAmount The amount of currency in the pack.
     @param currencyItemId The itemId of the currency associated with this
            pack.
     @param purchaseType see parent.
     @return The currency pack.
    */
    static CCVirtualCurrencyPack*
    create(const std::string& name, const std::string& description,
           const std::string& itemId, std::int32_t currencyAmount,
           const std::string& currencyItemId, CCPurchaseType* purchaseType);
    
    using CCPurchasableVirtualItem::init;
    
    bool init(const std::string& name, const std::string& description,
              const std::string& itemId, std::int32_t currencyAmount,
              const std::string& currencyItemId, CCPurchaseType* purchaseType);
    /**
     Creates a `CCVirtualCurrencyPack`.
     @param dict A dictionary containing keys to each of the parameters
            required by the create function.
     @return The pack.
    */
    SL_CREATE_WITH_VALUE_MAP(CCVirtualCurrencyPack);
    
    bool initWithValueMap(const cocos2d::ValueMap& map) override;
    /**
    Converts this `CCVirtualCurrencyPack` to a `CCDictionary`.
    @return `CCDictionary` representation of this `CCVirtualCurrencyPack`.
    */
    cocos2d::ValueMap toValueMap() override;

    virtual ~CCVirtualCurrencyPack() = default;

    virtual const char* getType() const override;

    /**
     See parent
     */
    virtual bool canBuy() override { return true; }

    /**
     See parent
     */
    virtual int give(int amount, bool notify, CCError** error = nullptr) override;

    /**
     See parent
     */
    virtual int take(int amount, bool notify, CCError** error = nullptr) override;

    /**
     See parent
     */
    virtual int resetBalance(int balance, bool notify,
                             CCError** error = nullptr) override;
    /**
     See parent
     */
    virtual int getBalance(CCError** error = nullptr) override;

    CC_SYNTHESIZE(std::int32_t, mCurrencyAmount, CurrencyAmount);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mCurrencyItemId, CurrencyItemId);

protected:
    void fillCurrencyAmountFromValueMap(const cocos2d::ValueMap& map);
    void putCurrencyAmountToValueMap(cocos2d::ValueMap& map);
    void fillCurrencyItemIdFromValueMap(const cocos2d::ValueMap& map);
    void putCurrencyItemIdToValueMap(cocos2d::ValueMap& map);
};
}; // namespace soomla

#endif //__CCVirtualCurrencyPack_H_
