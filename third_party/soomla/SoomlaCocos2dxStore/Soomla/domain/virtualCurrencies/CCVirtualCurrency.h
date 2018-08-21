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

//  Created by Igor Yegoroff on 5/17/13.

#ifndef __CCVirtualCurrency_H__
#define __CCVirtualCurrency_H__

#include "CCVirtualItem.h"

namespace soomla {

/**
 @class CCVirtualCurrency
 @brief A virtual currency.

 This is a representation of a game's virtual currency.
 Each game can have multiple instances of a virtual currency, all kept in
 `StoreInfo`.

 Real Game Examples: 'Coin', 'Gem', 'Muffin'

 NOTE: This item is NOT purchasable!
 However, a `CCVirtualCurrencyPack` IS purchasable.
 For example, if the virtual currency in your game is a 'Coin' and you want
 to make a single 'Coin' available for purchase you will need to define a
 `CCVirtualCurrencyPack` of 1 'Coin'.
 */
class CCVirtualCurrency : public CCVirtualItem {
public:
    CCVirtualCurrency()
        : CCVirtualItem() {}

    /**
     Creates a `CCVirtualCurrency`.
     @param name The currency's name.
     @param description The currency's description.
     @param itemId The currency's item id.
     @return The currency.
    */
    static CCVirtualCurrency* create(const std::string& name,
                                     const std::string& description,
                                     const std::string& itemId);
    /**
     Creates a `CCVirtualCurrency` from a dictionary.
     @param dict A dictionary containing keys to each of the parameters
            required by the create function.
     @return The currency.
    */
    SL_CREATE_WITH_VALUE_MAP(CCVirtualCurrency);

    virtual const char* getType() const override;

    /**
     See parent
     */
    virtual int give(int amount, bool notify, CCError** error = NULL) override;

    /**
     See parent
     */
    virtual int take(int amount, bool notify, CCError** error = NULL) override;

    /**
     See parent
     */
    virtual int resetBalance(int balance, bool notify,
                             CCError** error = NULL) override;
    /**
     See parent
     */
    virtual int getBalance(CCError** error = NULL) override;
};
} // namespace soomla

#endif /* defined(__CCVirtualCurrency_H__) */
