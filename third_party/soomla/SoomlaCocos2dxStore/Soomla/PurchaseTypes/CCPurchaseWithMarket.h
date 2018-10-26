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

#ifndef __CCPurchaseWithMarket_H_
#define __CCPurchaseWithMarket_H_

#include "../domain/CCMarketItem.h"
#include "CCPurchaseType.h"

namespace soomla {
/**
 @class CCPurchaseWithMarket
 @brief This type of Purchase is used to let users purchase
 `CCPurchasableVirtualItems` with real money.

 This type of Purchase is used to let users purchase
 `CCPurchasableVirtualItems` via the store (with real money $$$).
 */
class CCPurchaseWithMarket : public CCPurchaseType {
    CC_SYNTHESIZE_RETAIN(CCMarketItem*, mMarketItem, MarketItem);

public:
    CCPurchaseWithMarket()
        : mMarketItem(nullptr){};

    /**
     Creates a `CCPurchaseWithMarket`.
     @param productId The product id of the item in the native store.
     @param price The price of the item in the native store.
     @return The purchase with market type.
     */
    static CCPurchaseWithMarket* create(const std::string& productId, double price);
    /**
     Creates a `CCPurchaseWithMarket` from a `CCMarketItem`.
     @param marketItem A `CCMarketItem` that represents this item.
     @return The purchase with market type.
     */
    static CCPurchaseWithMarket* createWithMarketItem(CCMarketItem* marketItem);
 
    bool initWithMarketItem(CCMarketItem* marketItem);

    /**
     See parent
     */
    virtual void buy(const std::string& payload, CCError** error = nullptr) override;

    /**
     See parent
     */
    virtual bool canAfford(CCError** error = nullptr) override;

    ~CCPurchaseWithMarket();
};
}; // namespace soomla

#endif //__CCPurchaseWithMarket_H_
