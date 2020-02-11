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

#ifndef __CCUpgradeVG_H_
#define __CCUpgradeVG_H_

#include <soomla/CCSoomlaMacros.h>
#include "soomla/CCStoreConsts.h"

#include "soomla/domain/virtualGoods/CCLifetimeVG.h"
#include "soomla/domain/virtualGoods/CCVirtualGood.h"

namespace soomla {

/**
 @class CCUpgradeVG
 @brief An upgrade virtual good is one virtual good in a series of virtual
 goods that define an upgrade scale of an associated `CCVirtualGood`.

 This type of virtual good is best explained with an example:
 Let's say there's a "strength" attribute to one of the characters in your
 game and that strength is on a scale of 1-5. You want to provide your users
 with the ability to upgrade that strength.
 This is what you'll need to create:
 1. `CCSingleUseVG` for "strength"
 2. `CCUpgradeVG` for strength 'level 1'
 3. `CCUpgradeVG` for strength 'level 2'
 4. `CCUpgradeVG` for strength 'level 3'
 5. `CCUpgradeVG` for strength 'level 4'
 6. `CCUpgradeVG` for strength 'level 5'
 When the user buys this `UpgradeVG`, we check and make sure the appropriate
 conditions are met and buy it for you (which actually means we upgrade the
 associated `CCVirtualGood`).

 NOTE: In case you want this item to be available for purchase with real $$
 you'll need to define it in the Market (Google Play, App Store, etc...).

 Inheritance: CCUpgradeVG > CCLifeTimeVG > CCVirtualGood >
 CCPurchasableVirtualItem > CCVirtualItem
 */
class CCUpgradeVG : public CCLifetimeVG {
public:
    CCUpgradeVG()
        : CCLifetimeVG()
        , mGoodItemId("")
        , mPrevItemId("")
        , mNextItemId(""){};

    /**
     Creates a `CCUpgradeVG`.
     @param goodItemId The item id of the `CCVirtualGood` associated with
            this Upgrade. Id can be of any `CCVirtualGood` type.
     @param prevItemId The item id of the previous `CCUpgradeVG` in the
            series. Id MUST be a `CCUpgradeVG`'s itemId.
     @param nextItemId The item id of the following `CCUpgradeVG` in the
            series. Id MUST be a `CCUpgradeVG`'s itemId.
     @param name see parent.
     @param description see parent.
     @param itemId see parent.
     @param purchaseType see parent.
     @return The Upgrade virtual good.
     */
    static CCUpgradeVG*
    create(const std::string& goodItemId, const std::string& prevItemId,
           const std::string& nextItemId, const std::string& name,
           const std::string& description, const std::string& itemId,
           CCPurchaseType* purchaseType);

    /**
    Creates a `CCUpgradeVG` from a dictionary.
    @param dict A dictionary containing keys to each of the parameters
    required by the `create` function.
    @return The Upgrade virtual good.
    */
    SL_CREATE_WITH_VALUE_MAP(CCUpgradeVG);

    using CCLifetimeVG::init;

    bool init(const std::string& goodItemId, const std::string& prevItemId,
              const std::string& nextItemId, const std::string& name,
              const std::string& description, const std::string& itemId,
              CCPurchaseType* purchaseType);

    bool initWithValueMap(const cocos2d::ValueMap& map) override;

    /**
    Creates a `CCUpgradeVG` from a dictionary.
    @param dict A dictionary containing keys to each of the parameters
    required by the `create` function.
    @return The Upgrade virtual good.
    */
    cocos2d::ValueMap toValueMap() override;

    /**
     See parent
     */
    virtual bool canBuy() override;

    /**
     See parent
     */
    virtual int give(int amount, bool notify, CCError** error = nullptr) override;

    /**
     See parent
     */
    virtual int take(int amount, bool notify, CCError** error = nullptr) override;

    virtual ~CCUpgradeVG() = default;

    virtual const char* getType() const override;

    CC_SYNTHESIZE(std::string, mGoodItemId, GoodItemId);
    CC_SYNTHESIZE(std::string, mPrevItemId, PrevItemId);
    CC_SYNTHESIZE(std::string, mNextItemId, NextItemId);
protected:
    void fillGoodItemIdFromValueMap(const cocos2d::ValueMap& map);
    void putGoodItemIdToValueMap(cocos2d::ValueMap& map);
    void fillPrevItemIdFromValueMap(const cocos2d::ValueMap& map);
    void putPrevItemIdToValueMap(cocos2d::ValueMap& map);
    void fillNextItemIdFromValueMap(const cocos2d::ValueMap& map);
    void putNextItemIdToValueMap(cocos2d::ValueMap& map);
};
}; // namespace soomla

#endif //__CCUpgradeVG_H_
