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

#ifndef __CCSingleUsePackVG_H_
#define __CCSingleUsePackVG_H_

#include "CCStoreConsts.h"
#include "CCVirtualGood.h"

namespace soomla {
/**
 @class CCSingleUsePackVG
 @brief A pack of single use virtual goods.

 Single Use Packs are just bundles of `SingleUseVG`s.
 This kind of virtual good can be used to let your users buy more than one
 `SingleUseVG` at once.

 The `SingleUsePackVG`'s characteristics are:
 1. Can be purchased an unlimited number of times.
 2. Doesn't have a balance in the database. The `SingleUseVG` that's
 associated with this pack has its own balance. When your users buy a
 `SingleUsePackVG`, the balance of the associated `SingleUseVG` goes up in
 the amount that this pack represents.

 Real Game Examples: 'Box Of Chocolates', '10 Swords'

 NOTE: In case you want this item to be available for purchase with real $$
 you'll need to define it in the Market (Google Play, App Store, etc...).

 Inheritance: CCSingleUsePackVG > CCVirtualGood > CCPurchasableVirtualItem >
 CCVirtualItem
 */
class CCSingleUsePackVG : public CCVirtualGood {
public:
    CCSingleUsePackVG()
        : CCVirtualGood()
        , mGoodItemId("")
        , mGoodAmount(0){};

    static CCSingleUsePackVG*
    create(const std::string& goodItemId, std::int32_t goodAmount,
           const std::string& name, const std::string& description,
           const std::string& itemId, CCPurchaseType* purchaseType);
    /**
    Creates a `CCSingleUsePackVG` from a dictionary.
    @param dict A dictionary containing keys to each of the parameters
    required by the create function.
    @return The Sinlge Use Pack good.
    */
    SL_CREATE_WITH_VALUE_MAP(CCSingleUsePackVG);
    /**
    Creates a `CCSingleUsePackVG`.
    @param goodItemId The item id of the Single Use item associated with
    this Pack.
    @param goodAmount The amount of Single Use items in the Pack.
    @param name see parent.
    @param description see parent.
    @param itemId see parent.
    @param purchaseType see parent.
    @return The Sinlge Use Pack good.
    */
    bool init(const std::string& goodItemId, std::int32_t goodAmount,
              const std::string& name, const std::string& description,
              const std::string& itemId, CCPurchaseType* purchaseType);

    bool initWithValueMap(const cocos2d::ValueMap& map) override;
    /**
    Converts this `CCSingleUsePackVG` to a `CCDictionary`.
    @return `CCDictionary` representation of this `CCSingleUsePackVG`.
    */
    cocos2d::ValueMap toValueMap() override;

    virtual ~CCSingleUsePackVG() = default;

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

    virtual const char* getType() const override;

    CC_SYNTHESIZE_PASS_BY_REF(std::string, mGoodItemId, GoodItemId);
    CC_SYNTHESIZE(std::int32_t, mGoodAmount, GoodAmount);

protected:
    void fillGoodItemIdFromValueMap(const cocos2d::ValueMap& map);
    void putGoodItemIdToValueMap(cocos2d::ValueMap& map);
    void fillGoodAmountFromValueMap(const cocos2d::ValueMap& map);
    void putGoodAmountToValueMap(cocos2d::ValueMap& map);
};
}; // namespace soomla

#endif //__CCSingleUsePackVG_H_
