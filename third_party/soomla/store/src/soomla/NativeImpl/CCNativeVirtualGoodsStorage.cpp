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

#include "soomla/NativeImpl/CCNativeVirtualGoodsStorage.h"

#include <soomla/CCNdkBridge.h>
#include <soomla/CCSoomlaUtils.h>

#include "soomla/data/CCStoreInfo.h"
#include "soomla/domain/virtualGoods/CCUpgradeVG.h"

namespace soomla {
#define TAG "SOOMLA NativeVirtualGoodsStorage"

USING_NS_CC;

CCNativeVirtualGoodsStorage::CCNativeVirtualGoodsStorage() {}

CCNativeVirtualGoodsStorage::~CCNativeVirtualGoodsStorage() {}

int CCNativeVirtualGoodsStorage::getBalance(CCVirtualItem* item,
                                            CCError** error) {
    auto&& itemId = item->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling getBalance with: %s",
                                 itemId.c_str()));
    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::getBalance";
    params["itemId"] = itemId;

    auto&& retParams = CCNdkBridge::callNative(params, error);
    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}

int CCNativeVirtualGoodsStorage::setBalance(CCVirtualItem* item, int balance,
                                            bool notify, CCError** error) {
    auto&& itemId = item->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling setBalance with: %s",
                                 itemId.c_str()));
    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::setBalance";
    params["itemId"] = itemId;
    params["balance"] = balance;
    params["notify"] = notify;

    ValueMap&& retParams = CCNdkBridge::callNative(params, error);
    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}

int CCNativeVirtualGoodsStorage::add(CCVirtualItem* item, int amount,
                                     bool notify, CCError** error) {
    auto&& itemId = item->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling add with: %s",
                                 itemId.c_str()));
    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::add";
    params["itemId"] = itemId;
    params["amount"] = amount;
    params["notify"] = notify;

    ValueMap&& retParams = CCNdkBridge::callNative(params, error);
    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}

int CCNativeVirtualGoodsStorage::remove(CCVirtualItem* item, int amount,
                                        bool notify, CCError** error) {
    auto&& itemId = item->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling remove with: %s",
                                 itemId.c_str()));
    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::remove";
    params["itemId"] = itemId;
    params["amount"] = amount;
    params["notify"] = notify;

    auto&& retParams = CCNdkBridge::callNative(params, error);
    if (retParams.count("return")) {
        return retParams.at("return").asInt();
    }
    return 0;
}

void CCNativeVirtualGoodsStorage::removeUpgrades(CCVirtualGood* good,
                                                 bool notify, CCError** error) {
    auto&& itemId = good->getId();

    CCSoomlaUtils::logDebug(
        TAG,
        StringUtils::format("SOOMLA/COCOS2DX Calling removeUpgrades with: %s",
                            itemId.c_str()));

    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::removeUpgrades";
    params["itemId"] = itemId;
    params["notify"] = notify;
    CCNdkBridge::callNative(params, error);
}

void CCNativeVirtualGoodsStorage::assignCurrentUpgrade(CCVirtualGood* good,
                                                       CCUpgradeVG* upgradeVG,
                                                       bool notify,
                                                       CCError** error) {
    auto&& itemId = good->getId();
    auto&& upgradeItemId = upgradeVG->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "SOOMLA/COCOS2DX Calling assignCurrentUpgrade with: %s",
                 itemId.c_str()));

    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::assignCurrentUpgrade";
    params["itemId"] = itemId;
    params["upgradeItemId"] = upgradeItemId;
    params["notify"] = notify;
    CCNdkBridge::callNative(params, error);
}

CCUpgradeVG* CCNativeVirtualGoodsStorage::getCurrentUpgrade(CCVirtualGood* good,
                                                            CCError** error) {
    auto&& itemId = good->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format(
                 "SOOMLA/COCOS2DX Calling getCurrentUpgrade with: %s",
                 itemId.c_str()));

    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::getCurrentUpgrade";
    params["itemId"] = "itemId";

    auto&& retParams = CCNdkBridge::callNative(params, error);
    if (retParams.count("return")) {
        return dynamic_cast<CCUpgradeVG*>(
            CCStoreInfo::sharedStoreInfo()->getItemByItemId(
                retParams.at("return").asString(), error));
    }
    return nullptr;
}

bool CCNativeVirtualGoodsStorage::isEquipped(CCEquippableVG* good,
                                             CCError** error) {
    auto&& itemId = good->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling isEquipped with: %s",
                                 itemId.c_str()));

    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::isEquipped";
    params["itemId"] = itemId;

    auto&& retParams = CCNdkBridge::callNative(params, error);
    if (retParams.count("return")) {
        return retParams.at("return").asBool();
    }
    return false;
}

void CCNativeVirtualGoodsStorage::equip(CCEquippableVG* good, bool notify,
                                        CCError** error) {
    auto&& itemId = good->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling equip with: %s",
                                 itemId.c_str()));

    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::equip";
    params["itemId"] = itemId;
    params["notify"] = notify;
    CCNdkBridge::callNative(params, error);
}

void CCNativeVirtualGoodsStorage::unequip(CCEquippableVG* good, bool notify,
                                          CCError** error) {
    auto&& itemId = good->getId();

    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("SOOMLA/COCOS2DX Calling unequip with: %s",
                                 itemId.c_str()));

    ValueMap params;
    params["method"] = "CCNativeVirtualGoodsStorage::unequip";
    params["itemId"] = itemId;
    params["notify"] = notify;
    CCNdkBridge::callNative(params, error);
}
} // namespace soomla
