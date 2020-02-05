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

#include "Soomla/NativeImpl/CCNativeSoomlaStore.h"

#include <Soomla/CCNdkBridge.h>

namespace soomla {

USING_NS_CC;

void CCNativeSoomlaStore::buyMarketItem(const std::string& productId,
                                        const std::string& payload,
                                        CCError** error) {
    ValueMap params;
    params["method"] = "CCSoomlaStore::buyMarketItem";
    params["productId"] = productId;
    params["payload"] = payload;
    CCNdkBridge::callNative(params, error);
}

void CCNativeSoomlaStore::restoreTransactions() {
    ValueMap params;
    params["method"] = "CCSoomlaStore::restoreTransactions";
    CCNdkBridge::callNative(params, nullptr);
}

void CCNativeSoomlaStore::refreshInventory() {
    ValueMap params;
    params["method"] = "CCSoomlaStore::refreshInventory";
    CCNdkBridge::callNative(params, nullptr);
}

void CCNativeSoomlaStore::refreshMarketItemsDetails(CCError** error) {
    ValueMap params;
    params["method"] = "CCSoomlaStore::refreshMarketItemsDetails";
    CCNdkBridge::callNative(params, error);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
bool CCNativeSoomlaStore::transactionsAlreadyRestored() {
    ValueMap params;
    params["method"] = "CCSoomlaStore::transactionsAlreadyRestored";
    auto&& retParams = CCNdkBridge::callNative(params, nullptr);
    auto retValue = retParams.at("return").asBool();
    return retValue;
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CCNativeSoomlaStore::startIabServiceInBg() {
    ValueMap params;
    params["method"] = "CCSoomlaStore::startIabServiceInBg";
    CCNdkBridge::callNative(params, nullptr);
}

void CCNativeSoomlaStore::stopIabServiceInBg() {
    ValueMap params;
    params["method"] = "CCSoomlaStore::stopIabServiceInBg";
    CCNdkBridge::callNative(params, nullptr);
}
#endif

void CCNativeSoomlaStore::loadBillingService() {
    ValueMap params;
    params["method"] = "CCSoomlaStore::loadBillingService";
    CCNdkBridge::callNative(params, nullptr);
}
} // namespace soomla
