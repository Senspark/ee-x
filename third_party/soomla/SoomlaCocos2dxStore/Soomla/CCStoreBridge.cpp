

#include "CCDomainFactory.h"
#include "CCDomainHelper.h"
#include "CCEquippableVG.h"
#include "CCMarketItem.h"
#include "CCNativeStoreBridge.h"
#include "CCSingleUsePackVG.h"
#include "CCSingleUseVG.h"
#include "CCSoomlaStore.h"
#include "CCStoreBridge.h"
#include "CCStoreConsts.h"
#include "CCStoreEventDispatcher.h"
#include "CCUpgradeVG.h"
#include "CCVirtualCategory.h"
#include "CCVirtualCurrency.h"
#include "CCVirtualCurrencyPack.h"
#include "CCVirtualItem.h"
#include "CCVirtualItemReward.h"

namespace soomla {

USING_NS_CC;

#define TAG "SOOMLA CCStoreBridge"

static CCStoreBridge* sInstance = nullptr;

CCStoreBridge* CCStoreBridge::getInstance() {
    if (!sInstance) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        sInstance = new CCNativeStoreBridge();
#else
        sInstance = new CCStoreBridge();
#endif
        sInstance->retain();
    }
    return sInstance;
}

void CCStoreBridge::initShared() {
    CCStoreBridge* storeBridge = CCStoreBridge::getInstance();
    if (!storeBridge->init()) {
        exit(1);
    }
}

bool CCStoreBridge::init() {
    CCStoreEventDispatcher::getInstance(); // to get sure it's inited

    CCDomainFactory* domainFactory = CCDomainFactory::getInstance();
    domainFactory->registerCreator(CCStoreConsts::JSON_JSON_TYPE_MARKET_ITEM,
                                   CCMarketItem::createWithValueMap);
    domainFactory->registerCreator(
        CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CATEGORY,
        CCVirtualCategory::createWithValueMap);
    domainFactory->registerCreator(
        CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY,
        CCVirtualCurrency::createWithValueMap);
    domainFactory->registerCreator(
        CCStoreConsts::JSON_JSON_TYPE_VIRTUAL_CURRENCY_PACK,
        CCVirtualCurrencyPack::createWithValueMap);
    domainFactory->registerCreator(CCStoreConsts::JSON_JSON_TYPE_EQUIPPABLE_VG,
                                   CCEquippableVG::createWithValueMap);
    domainFactory->registerCreator(CCStoreConsts::JSON_JSON_TYPE_LIFETIME_VG,
                                   CCLifetimeVG::createWithValueMap);
    domainFactory->registerCreator(
        CCStoreConsts::JSON_JSON_TYPE_SINGLE_USE_PACK_VG,
        CCSingleUsePackVG::createWithValueMap);
    domainFactory->registerCreator(CCStoreConsts::JSON_JSON_TYPE_SINGLE_USE_VG,
                                   CCSingleUseVG::createWithValueMap);
    domainFactory->registerCreator(
        CCStoreConsts::JSON_JSON_TYPE_UPGRADE_VG,
        CCUpgradeVG::CCUpgradeVG::createWithValueMap);

    domainFactory->registerCreator(CCStoreConsts::JSON_JSON_TYPE_ITEM,
                                   &CCVirtualItemReward::createWithValueMap);

    return true;
}

void CCStoreBridge::applyParams(const cocos2d::ValueMap& storeParams) {}
} // namespace soomla
