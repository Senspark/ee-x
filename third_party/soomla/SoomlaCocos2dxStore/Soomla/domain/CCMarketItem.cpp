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

#include "CCMarketItem.h"
#include "CCJsonHelper.h"

USING_NS_CC;

namespace soomla {

CCMarketItem* CCMarketItem::create(const std::string& productId, double price) {
    CCMarketItem* ret = new CCMarketItem();
    if (ret->init(productId, price)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCMarketItem::init(const std::string& productId, double price) {
    setProductId(productId);
    setPrice(price);
    return true;
}

bool CCMarketItem::initWithValueMap(const cocos2d::ValueMap& dict) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    char const* key = CCStoreConsts::JSON_MARKET_ITEM_ANDROID_ID;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    char const* key = CCStoreConsts::JSON_MARKET_ITEM_IOS_ID;
#else
    char const* key = CCStoreConsts::JSON_MARKET_ITEM_PRODUCT_ID;
#endif
    CCASSERT(dict.count(key), "invalid object type in dictionary");
    if (dict.count(key)) {
        setProductId(dict.at(key).asString());
    } else {
        fillProductIdFromValueMap(dict);
    }

    fillPriceFromValueMap(dict);

    fillMarketPriceAndCurrencyFromValueMap(dict);
    fillMarketTitleFromValueMap(dict);
    fillMarketDescriptionFromValueMap(dict);
    fillMarketCurrencyCodeFromValueMap(dict);

    if (dict.count(CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE_MICROS)) {
        auto item = dict.at(CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE_MICROS)
                        .asDouble();
        if (mMarketPriceMicros != item) {
            setMarketPriceMicros((std::int32_t)item);
        }
    } else {
        fillMarketPriceMicrosFromValueMap(dict);
    }

    return true;
}

cocos2d::ValueMap CCMarketItem::toValueMap() {
    ValueMap map;
    
    putProductIdToValueMap(map);
    putPriceToValueMap(map);

    putMarketPriceAndCurrencyToValueMap(map);
    putMarketTitleToValueMap(map);
    putMarketDescriptionToValueMap(map);
    putMarketCurrencyCodeToValueMap(map);
    putMarketPriceMicrosToValueMap(map);
    
    return map;
}

void CCMarketItem::fillProductIdFromValueMap(const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_MARKET_ITEM_PRODUCT_ID)) {
        setProductId(
            map.at(CCStoreConsts::JSON_MARKET_ITEM_PRODUCT_ID).asString());
    } else {
        setProductId("");
    }
}

void CCMarketItem::putProductIdToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_MARKET_ITEM_PRODUCT_ID] = getProductId();
}

void CCMarketItem::fillPriceFromValueMap(const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_MARKET_ITEM_PRICE)) {
        setPrice(map.at(CCStoreConsts::JSON_MARKET_ITEM_PRICE).asDouble());
    } else {
        setPrice(0.);
    }
}

void CCMarketItem::putPriceToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_MARKET_ITEM_PRICE] = getPrice();
}

void CCMarketItem::fillMarketPriceAndCurrencyFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE)) {
        setMarketPriceAndCurrency(
            map.at(CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE).asString());
    } else {
        setMarketPriceAndCurrency("");
    }
}

void CCMarketItem::putMarketPriceAndCurrencyToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE] =
        getMarketPriceAndCurrency();
}

void CCMarketItem::fillMarketTitleFromValueMap(const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_MARKET_ITEM_MARKET_TITLE)) {
        setMarketTitle(
            map.at(CCStoreConsts::JSON_MARKET_ITEM_MARKET_TITLE).asString());
    } else {
        setMarketTitle("");
    }
}

void CCMarketItem::putMarketTitleToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_MARKET_ITEM_MARKET_TITLE] = getMarketTitle();
}

void CCMarketItem::fillMarketDescriptionFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_MARKET_ITEM_MARKET_DESCRIPTION)) {
        setMarketDescription(
            map.at(CCStoreConsts::JSON_MARKET_ITEM_MARKET_DESCRIPTION)
                .asString());
    } else {
        setMarketDescription("");
    }
}

void CCMarketItem::putMarketDescriptionToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_MARKET_ITEM_MARKET_DESCRIPTION] =
        getMarketDescription();
}

void CCMarketItem::fillMarketCurrencyCodeFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_MARKET_ITEM_MARKET_CURRENCY_CODE)) {
        setMarketCurrencyCode(
            map.at(CCStoreConsts::JSON_MARKET_ITEM_MARKET_CURRENCY_CODE)
                .asString());
    } else {
        setMarketCurrencyCode("");
    }
}

void CCMarketItem::putMarketCurrencyCodeToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_MARKET_ITEM_MARKET_CURRENCY_CODE] =
        getMarketCurrencyCode();
}

void CCMarketItem::fillMarketPriceMicrosFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE_MICROS)) {
        setMarketPriceMicros(
            map.at(CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE_MICROS)
                .asDouble());
    } else {
        setMarketPriceMicros(0.);
    }
}

void CCMarketItem::putMarketPriceMicrosToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_MARKET_ITEM_MARKET_PRICE_MICROS] =
        getMarketPriceMicros();
}

}; // namespace soomla
