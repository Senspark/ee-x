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

#ifndef __CCPurchasableVirtualItem__
#define __CCPurchasableVirtualItem__

#include "Soomla/domain/CCVirtualItem.h"
#include "Soomla/PurchaseTypes/CCPurchaseType.h"

namespace soomla {
/**
 @class CCPurchasableVirtualItem
 @brief A purchasable virtual item.

 A representation of a `CCVirtualItem` that you can actually purchase
 (eitherwith real money or with other virtual items).
*/
class CCPurchasableVirtualItem : public CCVirtualItem {
public:
    CCPurchasableVirtualItem()
        : CCVirtualItem()
        , mPurchaseType(nullptr) {}

    using CCVirtualItem::init;

    virtual bool init(const std::string& name, const std::string& description,
                      const std::string& itemId, CCPurchaseType* purchaseType);
    virtual bool initWithValueMap(const cocos2d::ValueMap& map) override;

    virtual ~CCPurchasableVirtualItem();

    /**
     Checks if there is enough funds to afford the `CCPurchasableVirtualItem`.
     This action uses the associated `CCPurchaseType` to perform the check.
     @param error A `CCError` for error checking.
     @return True if there are enough funds to afford the virtual item
     with the given item id
     */
    bool canAfford(CCError** error = nullptr);

    /**
     Buys the `CCPurchasableVirtualItem`, after checking if the user is in a
     state that allows him/her to buy. This action uses the associated
     `CCPurchaseType` to perform the purchase.

     @param payload a string you want to be assigned to the purchase.
     This string is saved in a static variable and will be given bacl to you
     when the purchase is completed..
     @param error A `CCError` for error checking.
     */
    void buy(const std::string& payload, CCError** error = nullptr);

    /**
     Determines if user is in a state that allows him/her to buy a specific
     `CCVirtualItem`.
     */
    virtual bool canBuy() { return false; }

    /**
    Converts this `CCPurchasableVirtualItem` to a `CCDictionary`.
    @return `CCDictionary` representation of this
    `CCPurchasableVirtualItem`.
    */
    virtual cocos2d::ValueMap toValueMap() override;

    CC_SYNTHESIZE_RETAIN(CCPurchaseType*, mPurchaseType, PurchaseType);
protected:
    void fillPurchaseTypeFromValueMap(const cocos2d::ValueMap& map);
    void putPurchaseTypeToValueMap(cocos2d::ValueMap& map);
};
} // namespace soomla

#endif // __CCPurchasableVirtualItem__
