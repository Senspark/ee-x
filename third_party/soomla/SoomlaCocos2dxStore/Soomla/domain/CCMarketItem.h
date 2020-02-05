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

#ifndef __CCMarketItem_H_
#define __CCMarketItem_H_

#include <cocos2d.h>

#include <Soomla/CCSoomlaMacros.h>
#include "Soomla/CCStoreConsts.h"
#include <Soomla/domain/CCDomain.h>

namespace soomla {
/**
 @class CCMarketItem
 @brief An item you can purchase in the market (App Store, Google Play...).

 This class represents an item in the market (App Store, Google Play...).
 Every `CCPurchasableVirtualItem` with `CCPurchaseType` of
 `CCPurchaseWithMarket` has an instance of this class.
 */
class CCMarketItem : public CCDomain {
public:
    CCMarketItem()
        : mProductId("")
        , mPrice(0.)
        , mMarketPriceAndCurrency("")
        , mMarketTitle("")
        , mMarketDescription("")
        , mMarketCurrencyCode("")
        , mMarketPriceMicros(0.){};

    /**
     Creates a `CCMarketItem`.
     @param productId The item's id in the native store.
     @param price The default price of the item, for display when connection
     to the market is not available
     @return The Market item.
     */
    static CCMarketItem* create(const std::string& productId, double price);

    /**
     Creates a `CCMarketItem` from a dictionary.
     @param dict A dictionary containing keys for the item's product id,
            and price.
     @return The Market item.
    */
    SL_CREATE_WITH_VALUE_MAP(CCMarketItem);

    bool init(const std::string& productId, double price);

    virtual bool initWithValueMap(const cocos2d::ValueMap& map) override;

    virtual ~CCMarketItem() = default;

    /**
    Converts this `CCMarketItem` to a `CCDictionary`.
    @return `CCDictionary` representation of this `CCMarketItem`.
    */
    virtual cocos2d::ValueMap toValueMap() override;

    CC_SYNTHESIZE_PASS_BY_REF(std::string, mProductId, ProductId);
    CC_SYNTHESIZE(double, mPrice, Price);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mMarketPriceAndCurrency,
                              MarketPriceAndCurrency);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mMarketTitle, MarketTitle);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mMarketDescription,
                              MarketDescription);
    CC_SYNTHESIZE_PASS_BY_REF(std::string, mMarketCurrencyCode,
                              MarketCurrencyCode);
    CC_SYNTHESIZE(double, mMarketPriceMicros, MarketPriceMicros);
protected:
    void fillProductIdFromValueMap(const cocos2d::ValueMap& map);
    void putProductIdToValueMap(cocos2d::ValueMap& map);
    void fillPriceFromValueMap(const cocos2d::ValueMap& map);
    void putPriceToValueMap(cocos2d::ValueMap& map);
    void fillMarketPriceAndCurrencyFromValueMap(const cocos2d::ValueMap& map);
    void putMarketPriceAndCurrencyToValueMap(cocos2d::ValueMap& map);
    void fillMarketTitleFromValueMap(const cocos2d::ValueMap& map);
    void putMarketTitleToValueMap(cocos2d::ValueMap& map);
    void fillMarketDescriptionFromValueMap(const cocos2d::ValueMap& map);
    void putMarketDescriptionToValueMap(cocos2d::ValueMap& map);
    void fillMarketCurrencyCodeFromValueMap(const cocos2d::ValueMap& map);
    void putMarketCurrencyCodeToValueMap(cocos2d::ValueMap& map);
    void fillMarketPriceMicrosFromValueMap(const cocos2d::ValueMap& map);
    void putMarketPriceMicrosToValueMap(cocos2d::ValueMap& map);
};
}; // namespace soomla

#endif //__CCMarketItem_H_
