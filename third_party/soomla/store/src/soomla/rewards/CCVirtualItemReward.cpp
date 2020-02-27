#include "soomla/rewards/CCVirtualItemReward.h"

#include "soomla/CCStoreConsts.h"
#include <soomla/CCJsonHelper.h>
#include <soomla/CCSoomlaUtils.h>

#include "soomla/CCStoreInventory.h"

#define TAG "SOOMLA VirtualItemReward"

soomla::CCVirtualItemReward* soomla::CCVirtualItemReward::create(
    const std::string& rewardId, const std::string& name, std::int32_t amount,
    const std::string& associatedItemId) {
    auto ret = new CCVirtualItemReward();
    if (ret->init(rewardId, name, associatedItemId, amount)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool soomla::CCVirtualItemReward::init(const std::string& rewardId,
                                       const std::string& name,
                                       const std::string& associatedItemId,
                                       std::int32_t amount) {
    bool result = CCReward::init(rewardId, name, nullptr);
    if (result) {
        setAmount(amount);
        setAssociatedItemId(associatedItemId);
        return true;
    } else {
        return false;
    }
}

bool soomla::CCVirtualItemReward::initWithValueMap(
    const cocos2d::ValueMap& dict) {
    CCReward::initWithValueMap(dict);

    fillAmountFromValueMap(dict);
    fillAssociatedItemIdFromValueMap(dict);

    return true;
}

cocos2d::ValueMap soomla::CCVirtualItemReward::toValueMap() {
    auto&& map = CCReward::toValueMap();
    putAmountToValueMap(map);
    putAssociatedItemIdToValueMap(map);
    return map;
}

const char* soomla::CCVirtualItemReward::getType() const {
    return CCStoreConsts::JSON_JSON_TYPE_ITEM;
}

bool soomla::CCVirtualItemReward::takeInner() {
    CCError* error = nullptr;
    auto&& itemId = this->getAssociatedItemId();
    CCStoreInventory::sharedStoreInventory()->takeItem(
        itemId, this->getAmount(), &error);
    if (error) {
        CCSoomlaUtils::logError(
            TAG,
            cocos2d::StringUtils::format(
                "(take) Couldn't find associated itemId: %s", itemId.c_str()));
        CCSoomlaUtils::logError(TAG, error->getInfo());
        return false;
    }
    return true;
}

bool soomla::CCVirtualItemReward::giveInner() {
    CCError* error = nullptr;
    auto&& itemId = this->getAssociatedItemId();
    CCStoreInventory::sharedStoreInventory()->giveItem(
        itemId, this->getAmount(), &error);
    if (error) {
        CCSoomlaUtils::logError(
            TAG,
            cocos2d::StringUtils::format(
                "(give) Couldn't find associated itemId: %s", itemId.c_str()));
        CCSoomlaUtils::logError(TAG, error->getInfo());
        return false;
    }
    return true;
}

void soomla::CCVirtualItemReward::fillAmountFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_AMOUNT)) {
        setAmount(map.at(CCStoreConsts::JSON_AMOUNT).asInt());
    } else {
        setAmount(0);
    }
}

void soomla::CCVirtualItemReward::putAmountToValueMap(cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_AMOUNT] = getAmount();
}

void soomla::CCVirtualItemReward::fillAssociatedItemIdFromValueMap(
    const cocos2d::ValueMap& map) {
    if (map.count(CCStoreConsts::JSON_ASSOCIATED_ITEM_ID)) {
        setAssociatedItemId(
            map.at(CCStoreConsts::JSON_ASSOCIATED_ITEM_ID).asString());
    } else {
        setAssociatedItemId("");
    }
}

void soomla::CCVirtualItemReward::putAssociatedItemIdToValueMap(
    cocos2d::ValueMap& map) {
    map[CCStoreConsts::JSON_ASSOCIATED_ITEM_ID] = getAssociatedItemId();
}
