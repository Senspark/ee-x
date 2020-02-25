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

#include "soomla/PurchaseTypes/CCPurchaseWithVirtualItem.h"

#include <soomla/CCSoomlaUtils.h>
#include <soomla/CCStoreEventDispatcher.h>

#include "soomla/data/CCStoreInfo.h"
#include "soomla/data/CCVirtualCurrencyStorage.h"
#include "soomla/data/CCVirtualGoodsStorage.h"

namespace soomla {
USING_NS_CC;

#define TAG "SOOMLA PurchaseWithVirtualItem"

CCPurchaseWithVirtualItem*
CCPurchaseWithVirtualItem::create(const std::string& itemId,
                                  std::int32_t amount) {
    CCPurchaseWithVirtualItem* ret = new CCPurchaseWithVirtualItem();
    if (ret->init(itemId, amount)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool CCPurchaseWithVirtualItem::init(const std::string& itemId,
                                     std::int32_t amount) {
    setItemId(itemId);
    setAmount(amount);
    return true;
}

void CCPurchaseWithVirtualItem::buy(const std::string& payload,
                                    CCError** error) {
    auto&& associatedItemId = getAssociatedItemId();
    CCPurchasableVirtualItem* associatedItem =
        dynamic_cast<CCPurchasableVirtualItem*>(
            CCStoreInfo::sharedStoreInfo()->getItemByItemId(associatedItemId,
                                                            error));
    if (associatedItem == nullptr) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format(
                     "Trying to buy an non-existing associated item: %s",
                     associatedItemId.c_str())
                     .c_str());
        return;
    }

    auto&& targetItemId = getItemId();
    int amount = getAmount();
    CCSoomlaUtils::logDebug(
        TAG, StringUtils::format("Trying to buy a %s with %d pieces of %s",
                                 associatedItem->getName().c_str(), amount,
                                 targetItemId.c_str())
                 .c_str());

    CCVirtualItem* item =
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(targetItemId, error);
    if (item == nullptr) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format("Target virtual item %s doesn't exist !",
                                     targetItemId.c_str())
                     .c_str());
        return;
    }

    CCStoreEventDispatcher::getInstance()->onItemPurchaseStarted(associatedItem,
                                                                 true);

    int balance = item->getBalance(error);

    if (balance < amount) {
        auto&& errorValue =
            Value(StringUtils::format("You tried to buy with itemId: %s but "
                                      "you don't have enough funds to buy it.",
                                      item->getItemId().c_str()));
        CCError::tryFillError(error, errorValue, TAG);
        return;
    }

    item->take(amount);

    associatedItem->give(1);

    CCStoreEventDispatcher::getInstance()->onItemPurchased(associatedItem,
                                                           payload, true);
}

bool CCPurchaseWithVirtualItem::canAfford(CCError** error) {
    auto&& associatedItemId = getAssociatedItemId();
    CCPurchasableVirtualItem* associatedItem =
        dynamic_cast<CCPurchasableVirtualItem*>(
            CCStoreInfo::sharedStoreInfo()->getItemByItemId(associatedItemId,
                                                            error));
    if (associatedItem == nullptr) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format(
                     "Trying to buy an non-existing associated item: %s",
                     associatedItemId.c_str())
                     .c_str());
        return false;
    }

    auto&& targetItemId = getItemId();
    int amount = getAmount();
    CCSoomlaUtils::logDebug(
        TAG,
        StringUtils::format("Checking affordability of %s with %d pieces of %s",
                            associatedItem->getName().c_str(), amount,
                            targetItemId.c_str())
            .c_str());

    CCVirtualItem* item =
        CCStoreInfo::sharedStoreInfo()->getItemByItemId(targetItemId, error);
    if (item == nullptr) {
        CCSoomlaUtils::logError(
            TAG, StringUtils::format("Target virtual item %s doesn't exist !",
                                     targetItemId.c_str())
                     .c_str());
        return false;
    }

    int balance = item->getBalance(error);

    return balance >= amount;
}
} // namespace soomla
