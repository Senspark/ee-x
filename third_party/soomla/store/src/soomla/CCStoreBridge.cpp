#include "soomla/CCStoreBridge.h"

#include "soomla/CCDomainFactory.h"
#include "soomla/CCDomainHelper.h"
#include "soomla/CCSoomlaStore.h"
#include "soomla/CCStoreConsts.h"
#include "soomla/CCStoreEventDispatcher.h"
#include "soomla/domain/CCMarketItem.h"
#include "soomla/domain/CCVirtualItem.h"
#include "soomla/domain/CCVirtualCategory.h"
#include "soomla/domain/virtualGoods/CCEquippableVG.h"
#include "soomla/domain/virtualGoods/CCSingleUsePackVG.h"
#include "soomla/domain/virtualGoods/CCSingleUseVG.h"
#include "soomla/domain/virtualGoods/CCUpgradeVG.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrency.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"
#include "soomla/NativeImpl/CCNativeStoreBridge.h"
#include "soomla/rewards/CCVirtualItemReward.h"

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
