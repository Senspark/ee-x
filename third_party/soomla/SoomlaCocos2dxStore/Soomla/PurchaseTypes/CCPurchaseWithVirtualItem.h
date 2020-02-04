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

#ifndef __CCPurchaseWithVirtualItem_H_
#define __CCPurchaseWithVirtualItem_H_

#include "Soomla/PurchaseTypes/CCPurchaseType.h"

namespace soomla {
/**
 @class CCPurchaseWithVirtualItem
 @brief This type of Purchase is used to let users purchase
 `CCPurchasableVirtualItems` with other virtual items.

 This type of purchase allows users to purchase a virtual item with other
 virtual items.

 Real Game Example: Purchase a 'Sword' in exchange for 100 'Gem's. 'Sword'
 is the item to be purchased, 'Gem' is the target item, and 100 is the
 amount.
*/
class CCPurchaseWithVirtualItem : public CCPurchaseType {
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mItemId, ItemId);
    CC_SYNTHESIZE(std::int32_t, mAmount, Amount);

public:
    CCPurchaseWithVirtualItem()
        : mItemId("")
        , mAmount(0) {}

    /**
     Creates a `CCPurchaseWithVirtualItem`.
     @param itemId The itemId of the virtual item that is used to "pay" with
            in order to make the purchase.
     @param amount The amount of the payment item that is needed in order to
            make the purchase.
     @return The purchase with virtual item type.
    */
    static CCPurchaseWithVirtualItem* create(const std::string& itemId,
                                             std::int32_t amount);
    
    using CCPurchaseType::init;

    virtual bool init(const std::string& itemId, std::int32_t amount);

    /**
     See parent
     */
    virtual void buy(const std::string& payload, CCError** error = nullptr) override;
    
    /**
     See parent
     */
    virtual bool canAfford(CCError** error = nullptr) override;

    virtual ~CCPurchaseWithVirtualItem() = default;
};

}; // namespace soomla

#endif //__CCPurchaseWithVirtualItem_H_
