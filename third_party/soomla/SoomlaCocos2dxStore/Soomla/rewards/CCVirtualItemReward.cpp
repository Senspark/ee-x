
#include "CCJsonHelper.h"
#include "CCSoomlaUtils.h"
#include "CCStoreConsts.h"
#include "CCStoreInventory.h"
#include "CCVirtualItemReward.h"

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
    bool result = CCReward::init(rewardId, name);
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
    CCError* error = NULL;
    char const* itemId = this->getAssociatedItemId().c_str();
    CCStoreInventory::sharedStoreInventory()->takeItem(
        itemId, this->getAmount(), &error);
    if (error) {
        CCSoomlaUtils::logError(
            TAG, cocos2d::StringUtils::format(
                     "(take) Couldn't find associated itemId: %s", itemId)
                     .c_str());
        CCSoomlaUtils::logError(TAG, error->getInfo());
        return false;
    }
    return true;
}

bool soomla::CCVirtualItemReward::giveInner() {
    CCError* error = NULL;
    char const* itemId = this->getAssociatedItemId().c_str();
    CCStoreInventory::sharedStoreInventory()->giveItem(
        itemId, this->getAmount(), &error);
    if (error) {
        CCSoomlaUtils::logError(
            TAG, cocos2d::StringUtils::format(
                     "(give) Couldn't find associated itemId: %s", itemId)
                     .c_str());
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
