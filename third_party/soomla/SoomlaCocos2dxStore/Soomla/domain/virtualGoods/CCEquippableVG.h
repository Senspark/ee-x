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

#ifndef __CCEquippableVG_H_
#define __CCEquippableVG_H_

#include "CCLifetimeVG.h"

namespace soomla {
/**
 @class CCEquippableVG
 @brief An equippable virtual good.

 An Equippable virtual good is a special type of Lifetime virtual good that
 can be equipped by your users. Equipping means that the user decides to
 currently use a specific virtual good.

 The `CCEquippableVG`'s characteristics are:
 1. Can be purchased only once.
 2. Can be equipped by the user.
 3. Inherits the definition of `CCLifetimeVG`.

 There are 3 ways to equip an `CCEquippableVG`:
 1. LOCAL - The current `CCEquippableVG`'s equipping status doesn't affect
 any other `CCEquippableVG`.
 2. CATEGORY - In the containing category, if this `CCEquippableVG` is
 equipped, all other `CCEquippableVG`s will stay unequipped.
 3. GLOBAL - In the whole game, if this `CCEquippableVG` is equipped, all
 other `CCEquippableVG`s will stay unequipped.

 Real Game Examples:
 1. LOCAL: Say your game offers 3 weapons: a sword, a gun, and an axe
 (`LifetimeVG`s). Suppose your user has already bought all 3. These are
 euippables that do not affect one another - your user can “carry” the
 sword, gun, and axe at the same time if he/she chooses to!

 2. CATEGORY: Suppose your game offers “shirts” and “hats”. Let’s say there
 are 4 available shirts and 2 available hats, and your user has already
 purchased all of the clothing items available. The user can equip a shirt
 and a hat at the same time, but cannot equip more than 1 shirt or more than
 1 hat at the same time. In other words, the user can equip at most one of
 each category (shirts, hats)!

 3. GLOBAL: Suppose your game offers multiple characters (`CCLifetimeVGs`):
 RobotX and RobotY. Let’s say your user has bought both. In other words
 he/she owns both characters and will own them forever (because they are
 `CCLifetimeVG`s). Your user can only play as (i.e. Equip) one character at a
 time, either RobotX or RobotY, but never both at the same time!

 NOTE: In case you want this item to be available for purchase with real $$
 you'll need to define it in the Market (Google Play, App Store, etc...).

 Inheritance: CCEquippableVG > CCLifeTimeVG > CCVirtualGood >
 CCPurchasableVirtualItem > CCVirtualItem
 */
class CCEquippableVG : public CCLifetimeVG {
public:
    /**
     These are the available ways to equip a virtual good (explained above).
     `LOCAL`, `CATEGORY`, or `GLOBAL`.
     */
    typedef enum { kLocal = 0, kCategory = 1, kGlobal = 2 } EquippingModel;
public:
    CCEquippableVG()
        : CCLifetimeVG()
        , mEquippingModel(NULL){};

    /**
     Creates a `CCEquippableVG`.
     @param equippingModel Can be 0 (`LOCAL`), 1 (`CATEGORY`), or
            3 (`GLOBAL`)
     @param name see parent.
     @param description see parent.
     @param itemId see parent.
     @param purchaseType see parent.
     @return The Equippable virtual good.
     */
    static CCEquippableVG* create(std::int32_t equippingModel,
                                  const std::string& name,
                                  const std::string& description,
                                  const std::string& itemId,
                                  CCPurchaseType* purchaseType);

    bool init(std::int32_t equippingModel, const std::string& name,
              const std::string& description, const std::string& itemId,
              CCPurchaseType* purchaseType);

    bool initWithValueMap(const cocos2d::ValueMap& map) override;
    /**
     Creates a `CCEquippableVG` from a dictionary.
     @param dict A dictionary containing keys to each of the parameters
            required by the create function.
     @return The Equippable virtual good.
    */
    SL_CREATE_WITH_VALUE_MAP(CCEquippableVG);

    /**
    Converts this `CCEquippableVG` to a `CCDictionary`.
    @return `CCDictionary` representation of this `CCEquippableVG`.
    */
    cocos2d::ValueMap toValueMap() override;
    
    virtual ~CCEquippableVG() = default;

    /**
     Equips the current `CCEquippableVG`
     @param error If an error was generated during the execution it will
     be returned here
     */
    virtual void equip(CCError** error = NULL) { equip(true, error); }
    /**
     Equips the current `CCEquippableVG`.
     The equipping is done according to the equipping model ('GLOBAL',
     'CATEGORY', or 'LOCAL').
     @param notify if true, the relevant event will be posted when equipped.
     @param error If an error was generated during the execution it will
     be returned here
     */
    virtual void equip(bool notify, CCError** error = NULL);

    /**
     Unequips the current `CCEquippableVG`
     @param error If an error was generated during the execution it will
     be returned here
     */
    virtual void unequip(CCError** error = NULL) { unequip(true, error); }
    /**
     Unequips the current `CCEquippableVG`
     @param notify if true, the relevant event will be posted when equipped.
     @param error If an error was generated during the execution it will
     be returned here
     */
    virtual void unequip(bool notify, CCError** error = NULL);

    virtual const char* getType() const override;

    CC_SYNTHESIZE(std::int32_t, mEquippingModel, EquippingModel);
protected:
    void fillEquippingModelFromValueMap(const cocos2d::ValueMap& map);
    void putEquippingModelToValueMap(cocos2d::ValueMap& map);
};
}; // n
#endif
