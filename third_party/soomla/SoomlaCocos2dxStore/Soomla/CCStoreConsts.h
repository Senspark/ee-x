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

#ifndef __CCStoreConsts_H__
#define __CCStoreConsts_H__

/**
* This class contains all static const String names of the keys/vals in the
* JSON being parsed all around the SDK.
*/
namespace soomla {
    class CCStoreConsts {
    public:
        static char const *JSON_STORE_CURRENCIES;
        static char const *JSON_STORE_CURRENCY_PACKS;
        static char const *JSON_STORE_GOODS;
        static char const *JSON_STORE_CATEGORIES;
        static char const *JSON_STORE_GOODS_SU;
        static char const *JSON_STORE_GOODS_PA;
        static char const *JSON_STORE_GOODS_UP;
        static char const *JSON_STORE_GOODS_LT;
        static char const *JSON_STORE_GOODS_EQ;

        static char const *JSON_CATEGORY_NAME;
        static char const *JSON_CATEGORY_GOODS_ITEM_IDS;

        static char const *JSON_MARKET_ITEM_PRODUCT_ID;
        static char const *JSON_MARKET_ITEM_ANDROID_ID;
        static char const *JSON_MARKET_ITEM_IOS_ID;

        static char const *JSON_MARKET_ITEM_PRICE;

        static char const *JSON_MARKET_ITEM_MARKET_PRICE;
        static char const *JSON_MARKET_ITEM_MARKET_TITLE;
        static char const *JSON_MARKET_ITEM_MARKET_DESCRIPTION;
        static char const *JSON_MARKET_ITEM_MARKET_CURRENCY_CODE;
        static char const *JSON_MARKET_ITEM_MARKET_PRICE_MICROS;

        static char const *JSON_EQUIPPABLE_EQUIPPING;

        static char const *JSON_VGP_GOOD_ITEM_ID;
        static char const *JSON_VGP_GOOD_AMOUNT;

        static char const *JSON_VGU_GOOD_ITEM_ID;
        static char const *JSON_VGU_PREV_ITEM_ID;
        static char const *JSON_VGU_NEXT_ITEM_ID;

        static char const *JSON_CURRENCY_PACK_CURRENCY_AMOUNT;
        static char const *JSON_CURRENCY_PACK_CURRENCY_ITEM_ID;

        static char const *JSON_PURCHASABLE_ITEM;

        static char const *JSON_PURCHASE_TYPE;
        static char const *JSON_PURCHASE_TYPE_MARKET;
        static char const *JSON_PURCHASE_TYPE_VI;

        static char const *JSON_PURCHASE_MARKET_ITEM;

        static char const *JSON_PURCHASE_VI_ITEM_ID;
        static char const *JSON_PURCHASE_VI_AMOUNT;

        static char const *JSON_AMOUNT;
        static char const *JSON_ASSOCIATED_ITEM_ID;

        static char const *JSON_JSON_TYPE_MARKET_ITEM;
        static char const *JSON_JSON_TYPE_VIRTUAL_CATEGORY;
        static char const *JSON_JSON_TYPE_VIRTUAL_CURRENCY;
        static char const *JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK;
        static char const *JSON_JSON_TYPE_EQUIPPABLE_VG;
        static char const *JSON_JSON_TYPE_LIFETIME_VG;
        static char const *JSON_JSON_TYPE_SINGLE_USE_PACK_VG;
        static char const *JSON_JSON_TYPE_SINGLE_USE_VG;
        static char const *JSON_JSON_TYPE_UPGRADE_VG;
        static char const *JSON_JSON_TYPE_ITEM;
        
        /** Events **/
        
        static char const *DICT_ELEMENT_BALANCE;
        static char const *DICT_ELEMENT_CURRENCY;
        static char const *DICT_ELEMENT_AMOUNT_ADDED;
        static char const *DICT_ELEMENT_GOOD;
        static char const *DICT_ELEMENT_EQUIPPABLEVG;
        static char const *DICT_ELEMENT_UPGRADEVG;
        static char const *DICT_ELEMENT_PURCHASABLE;
        static char const *DICT_ELEMENT_PURCHASABLE_ID;
        static char const *DICT_ELEMENT_DEVELOPERPAYLOAD;
        static char const *DICT_ELEMENT_RECEIPT;
        static char const *DICT_ELEMENT_EXTRA_INFO;
        static char const *DICT_ELEMENT_ORIGINAL_JSON;
        static char const *DICT_ELEMENT_SIGNATURE;
        static char const *DICT_ELEMENT_USER_ID;
        static char const *DICT_ELEMENT_SUCCESS;
        static char const *DICT_ELEMENT_VERIFIED;
        static char const *DICT_ELEMENT_TRANSACTION;
        static char const *DICT_ELEMENT_ERROR_CODE;
        static char const *DICT_ELEMENT_ERROR_MESSAGE;
        static char const *DICT_ELEMENT_PRODUCTID;
        static char const *DICT_ELEMENT_PRICE;
        static char const *DICT_ELEMENT_TITLE;
        static char const *DICT_ELEMENT_DESCRIPTION;
        static char const *DICT_ELEMENT_MARKET_ITEMS;

        static char const *EVENT_BILLING_NOT_SUPPORTED;
        static char const *EVENT_BILLING_SUPPORTED;
        static char const *EVENT_CURRENCY_BALANCE_CHANGED;
        static char const *EVENT_GOOD_BALANCE_CHANGED;
        static char const *EVENT_GOOD_EQUIPPED;
        static char const *EVENT_GOOD_UNEQUIPPED;
        static char const *EVENT_GOOD_UPGRADE;
        static char const *EVENT_ITEM_PURCHASED;
        static char const *EVENT_ITEM_PURCHASE_STARTED;
        static char const *EVENT_MARKET_PURCHASE_CANCELED;
        static char const *EVENT_MARKET_PURCHASE_DEFERRED;
        static char const *EVENT_MARKET_PURCHASE;
        static char const *EVENT_MARKET_PURCHASE_STARTED;
        static char const *EVENT_MARKET_ITEMS_REFRESHED;
        static char const *EVENT_MARKET_ITEMS_REFRESH_STARTED;
        static char const *EVENT_MARKET_ITEMS_REFRESH_FAILED;
        static char const *EVENT_MARKET_PURCHASE_VERIFICATION;
        static char const *EVENT_RESTORE_TRANSACTION_FINISHED;
        static char const *EVENT_RESTORE_TRANSACTION_STARTED;
        static char const *EVENT_UNEXPECTED_STORE_ERROR;
        static char const *EVENT_VERIFICATION_STARTED;
        static char const *EVENT_SOOMLA_STORE_INITIALIZED;
        static char const *EVENT_MARKET_REFUND;
        static char const *EVENT_IAB_SERVICE_STARTED;
        static char const *EVENT_IAB_SERVICE_STOPPED;
    };
}

#endif /* __CCStoreConsts_H__ */
