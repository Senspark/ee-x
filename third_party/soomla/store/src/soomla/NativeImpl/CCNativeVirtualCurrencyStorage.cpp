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

#include "soomla/NativeImpl/CCNativeVirtualCurrencyStorage.h"

#include <soomla/CCNdkBridge.h>
#include <soomla/CCSoomlaUtils.h>

#include "soomla/data/CCStoreInfo.h"
#include "soomla/domain/virtualGoods/CCUpgradeVG.h"
#include "soomla/NativeImpl/CCNativeVirtualGoodsStorage.h"

namespace soomla {
#define TAG "SOOMLA NativeVirtualCurrencyStorage"

USING_NS_CC;

CCNativeVirtualCurrencyStorage::CCNativeVirtualCurrencyStorage() {}

CCNativeVirtualCurrencyStorage::~CCNativeVirtualCurrencyStorage() {}

int CCNativeVirtualCurrencyStorage::getBalance(CCVirtualItem* item,
                                               CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "SOOMLA/COCOS2DX Calling getBalance with: %s", item->getId().c_str())
                 .c_str());
    ValueMap params;
    params["method"] = "CCNativeVirtualCurrencyStorage::getBalance";
    params["itemId"] = item->getId();
    auto&& retParams = CCNdkBridge::callNative(params, error);

    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}

int CCNativeVirtualCurrencyStorage::setBalance(CCVirtualItem* item, int balance,
                                               bool notify, CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "SOOMLA/COCOS2DX Calling setBalance with: %s", item->getId().c_str())
                 .c_str());
    ValueMap params;
    params["method"] = "CCNativeVirtualCurrencyStorage::setBalance";
    params["itemId"] = item->getId();
    params["balance"] = balance;
    params["notify"] = notify;
    
    auto&& retParams = CCNdkBridge::callNative(params, error);
    
    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}

int CCNativeVirtualCurrencyStorage::add(CCVirtualItem* item, int amount,
                                        bool notify, CCError** error) {
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling add with: %s",
                                        item->getId().c_str())
                 .c_str());
    ValueMap params;
    params["method"] = "CCNativeVirtualCurrencyStorage::add";
    params["itemId"] = item->getId();
    params["amount"] = amount;
    params["notify"] = notify;
    auto&& retParams = CCNdkBridge::callNative(params, error);

    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}

int CCNativeVirtualCurrencyStorage::remove(CCVirtualItem* item, int amount,
                                           bool notify, CCError** error) {
    CCSoomlaUtils::logDebug(TAG, StringUtils::format(
                                     "SOOMLA/COCOS2DX Calling remove with: %s",
                                     item->getId().c_str())
                                     .c_str());
    ValueMap params;
    params["method"] = "CCNativeVirtualCurrencyStorage::remove";
    params["itemId"] = item->getId();
    params["amount"] = amount;
    params["notify"] = notify;
    
    auto&& retParams = CCNdkBridge::callNative(params, error);
    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}
} // namespace soomla
