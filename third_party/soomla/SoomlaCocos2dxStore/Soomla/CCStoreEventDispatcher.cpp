

#include "CCError.h"
#include "CCNdkBridge.h"
#include "CCPurchaseWithMarket.h"
#include "CCSoomlaEventDispatcher.h"
#include "CCSoomlaUtils.h"
#include "CCStoreEventDispatcher.h"
#include "CCStoreInfo.h"
#include "CCStoreInventory.h"

#if __has_include(<base/CCDirector.h>)
#define COCOSCREATOR_VERSION 1
#else
#define COCOSCREATOR_VERSION 2
#endif

#if COCOSCREATOR_VERSION == 2
#include "cocos/scripting/js-bindings/event/EventDispatcher.h"
#endif


namespace soomla {

USING_NS_CC;

static CCStoreEventDispatcher* s_SharedInstance = nullptr;

CCStoreEventDispatcher* CCStoreEventDispatcher::getInstance() {
    if (!s_SharedInstance) {
        s_SharedInstance = new CCStoreEventDispatcher();
        s_SharedInstance->init();
    }

    return s_SharedInstance;
}

bool CCStoreEventDispatcher::init() {
    CCSoomlaEventDispatcher* eventDispatcher =
        CCSoomlaEventDispatcher::getInstance();

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_BILLING_NOT_SUPPORTED,
        [this](const cocos2d::ValueMap& parameters) {
            this->onBillingNotSupported();
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_BILLING_SUPPORTED,
        [this](const cocos2d::ValueMap& parameters) {
            this->onBillingSupported();
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            auto balance = parameters.at("balance").asInt();
            auto amountAdded = parameters.at("amountAdded").asInt();
            CCError* error = nullptr;
            CCVirtualCurrency* virtualCurrency =
                dynamic_cast<CCVirtualCurrency*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED, error);
                return;
            }
            CC_ASSERT(virtualCurrency);
            this->onCurrencyBalanceChanged(virtualCurrency, balance,
                                           amountAdded);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            auto balance = parameters.at("balance").asInt();
            auto amountAdded = parameters.at("amountAdded").asInt();
            CCError* error = nullptr;
            CCVirtualGood* virtualGood = dynamic_cast<CCVirtualGood*>(
                CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED, error);
                return;
            }
            CC_ASSERT(virtualGood);
            this->onGoodBalanceChanged(virtualGood, balance, amountAdded);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_GOOD_EQUIPPED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCEquippableVG* equippableVG = dynamic_cast<CCEquippableVG*>(
                CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                &error));
            if (error) {
                CCSoomlaUtils::logException(CCStoreConsts::EVENT_GOOD_EQUIPPED,
                                            error);
                return;
            }
            CC_ASSERT(equippableVG);
            this->onGoodEquipped(equippableVG);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_GOOD_UNEQUIPPED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCEquippableVG* equippableVG = dynamic_cast<CCEquippableVG*>(
                CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_GOOD_UNEQUIPPED, error);
                return;
            }
            CC_ASSERT(equippableVG);
            this->onGoodUnEquipped(equippableVG);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_GOOD_UPGRADE,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            auto&& vguItemId = parameters.at("vguItemId").asString();

            CCError* error;

            error = nullptr;
            CCVirtualGood* virtualGood = dynamic_cast<CCVirtualGood*>(
                CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                &error));
            if (error) {
                CCSoomlaUtils::logException(CCStoreConsts::EVENT_GOOD_UPGRADE,
                                            error);
                return;
            }
            CC_ASSERT(virtualGood);

            error = nullptr;
            CCUpgradeVG* upgradeVG = nullptr;
            if (vguItemId.length() > 0) {
                upgradeVG = dynamic_cast<CCUpgradeVG*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(vguItemId,
                                                                    &error));
                if (error) {
                    CCSoomlaUtils::logException(
                        CCStoreConsts::EVENT_GOOD_UPGRADE, error);
                    return;
                }
                CC_ASSERT(upgradeVG);
            }
            this->onGoodUpgrade(virtualGood, upgradeVG);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_ITEM_PURCHASED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            auto&& payload = parameters.at("payload").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(CCStoreConsts::EVENT_ITEM_PURCHASED,
                                            error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onItemPurchased(purchasableVirtualItem, payload);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_ITEM_PURCHASE_STARTED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_ITEM_PURCHASE_STARTED, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onItemPurchaseStarted(purchasableVirtualItem);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onMarketPurchaseCancelled(purchasableVirtualItem);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_PURCHASE_DEFERRED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            auto&& payload = parameters.at("payload").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_MARKET_PURCHASE_DEFERRED, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onMarketPurchaseDeferred(purchasableVirtualItem, payload);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_PURCHASE,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_MARKET_PURCHASE, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            auto&& payload = parameters.at("payload").asString();
            auto& extraInfo = parameters.at("extraInfo").asValueMap();
            this->onMarketPurchase(purchasableVirtualItem, payload, extraInfo);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onMarketPurchaseStarted(purchasableVirtualItem);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onMarketPurchaseStarted(purchasableVirtualItem);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED,
        [this](const cocos2d::ValueMap& parameters) {
            auto& marketItemDicts =
                parameters.at("marketItems").asValueVector();
            Vector<CCMarketItem*> marketItems;
            Vector<CCVirtualItem*> virtualItems;

            CCError* error = nullptr;
            for (auto marketItemDict : marketItemDicts) {
                CC_ASSERT(marketItemDict.getType() ==
                          cocos2d::Value::Type::MAP);
                auto& marketItem = marketItemDict.asValueMap();
                auto&& productId = marketItem.at("productId").asString();
                auto&& marketPrice = marketItem.at("marketPrice").asString();
                auto&& marketTitle = marketItem.at("marketTitle").asString();
                auto&& marketDescription =
                    marketItem.at("marketDesc").asString();
                auto&& marketCurrencyCode =
                    marketItem.at("marketCurrencyCode").asString();
                auto& marketPriceMicros = marketItem.at("marketPriceMicros");

                CCPurchasableVirtualItem* pvi =
                    CCStoreInfo::sharedStoreInfo()
                        ->getPurchasableItemWithProductId(productId,
                                                          &error);
                if (error) {
                    CCSoomlaUtils::logException(
                        CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED, error);
                    return;
                }
                CC_ASSERT(pvi);

                CCPurchaseWithMarket* purchaseWithMarket =
                    dynamic_cast<CCPurchaseWithMarket*>(pvi->getPurchaseType());
                CC_ASSERT(purchaseWithMarket);
                CCMarketItem* mi = purchaseWithMarket->getMarketItem();
                mi->setMarketPriceAndCurrency(marketPrice);
                mi->setMarketTitle(marketTitle);
                mi->setMarketDescription(marketDescription);
                mi->setMarketCurrencyCode(marketCurrencyCode);

                if (marketPriceMicros.getType() == Value::Type::DOUBLE) {
                    mi->setMarketPriceMicros(marketPriceMicros.asDouble());
                } else {
                    mi->setMarketPriceMicros(marketPriceMicros.asInt());
                }

                marketItems.pushBack(mi);
                virtualItems.pushBack(pvi);
            }

            if (virtualItems.size() > 0) {
                // no need to save to DB since it's already saved in native
                // before this event is received
                CCStoreInfo::sharedStoreInfo()->saveItems(virtualItems, false);
            }

            this->onMarketItemsRefreshed(marketItems);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_STARTED,
        [this](const cocos2d::ValueMap& parameters) {
            this->onMarketItemsRefreshStarted();
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_FAILED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& errorMessage = parameters.at("errorMessage").asString();
            this->onMarketItemsRefreshFailed(errorMessage);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_VERIFICATION_STARTED,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_VERIFICATION_STARTED, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onVerificationStarted(purchasableVirtualItem);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_PURCHASE_VERIFICATION,
        [this](const cocos2d::ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(
                    CCStoreConsts::EVENT_MARKET_PURCHASE_VERIFICATION, error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onMarketPurchaseVerification(purchasableVirtualItem);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_RESTORE_TRANSACTION_FINISHED,
        [this](const cocos2d::ValueMap& parameters) {
            auto success = parameters.at("success").asBool();
            this->onRestoreTransactionsFinished(success);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_RESTORE_TRANSACTION_STARTED,
        [this](const cocos2d::ValueMap& parameters) {
            this->onRestoreTransactionsStarted();
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_UNEXPECTED_STORE_ERROR,
        [this](const cocos2d::ValueMap& parameters) {
            std::int32_t errorCode = parameters.at("errorCode").asInt();
            this->onUnexpectedStoreError(errorCode);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_SOOMLA_STORE_INITIALIZED,
        [this](const cocos2d::ValueMap& parameters) {
            this->onSoomlaStoreInitialized();
        });

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_MARKET_REFUND, [this](const ValueMap& parameters) {
            auto&& itemId = parameters.at("itemId").asString();
            CCError* error = nullptr;
            CCPurchasableVirtualItem* purchasableVirtualItem =
                dynamic_cast<CCPurchasableVirtualItem*>(
                    CCStoreInfo::sharedStoreInfo()->getItemByItemId(itemId,
                                                                    &error));
            if (error) {
                CCSoomlaUtils::logException(CCStoreConsts::EVENT_MARKET_REFUND,
                                            error);
                return;
            }
            CC_ASSERT(purchasableVirtualItem);
            this->onMarketRefund(purchasableVirtualItem);
        });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_IAB_SERVICE_STARTED,
        [this](const ValueMap& parameters) { this->onIabServiceStarted(); });

    eventDispatcher->registerEventHandler(
        CCStoreConsts::EVENT_IAB_SERVICE_STOPPED,
        [this](const ValueMap& parameters) { this->onIabServiceStopped(); });
#endif
    return true;
}

void dispatchCustomEvent(const std::string& eventName, ValueMap* eventData = nullptr) {
#if COCOSCREATOR_VERSION == 2
    cocos2d::CustomEvent event;
    event.name = eventName;
    if (eventData != nullptr) {
        event.args[0].ptrVal = eventData;
    }
    EventDispatcher::dispatchCustomEvent(event);
#elif COCOSCREATOR_VERSION == 1
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
        eventName, eventData);
#endif
}
    
void CCStoreEventDispatcher::onBillingNotSupported() {
    dispatchCustomEvent(CCStoreConsts::EVENT_BILLING_NOT_SUPPORTED);
}

void CCStoreEventDispatcher::onBillingSupported() {
    dispatchCustomEvent(CCStoreConsts::EVENT_BILLING_SUPPORTED);
}

void CCStoreEventDispatcher::onCurrencyBalanceChanged(
    CCVirtualCurrency* virtualCurrency, int balance, int amountAdded) {
    CCStoreInventory::sharedStoreInventory()->refreshOnCurrencyBalanceChanged(
        virtualCurrency, balance, amountAdded);

    ValueMap eventMap;
    eventMap[CCStoreConsts::DICT_ELEMENT_CURRENCY] = virtualCurrency->toValueMap();
    eventMap[CCStoreConsts::CCStoreConsts::DICT_ELEMENT_BALANCE] = balance;
    eventMap[CCStoreConsts::DICT_ELEMENT_AMOUNT_ADDED] = amountAdded;
    
    dispatchCustomEvent(CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED, &eventMap);
}

void CCStoreEventDispatcher::onGoodBalanceChanged(CCVirtualGood* virtualGood,
                                                  int balance,
                                                  int amountAdded) {
    CCStoreInventory::sharedStoreInventory()->refreshOnGoodBalanceChanged(
        virtualGood, balance, amountAdded);

    ValueMap eventMap;
    eventMap[CCStoreConsts::DICT_ELEMENT_GOOD] = virtualGood->toValueMap();
    eventMap[CCStoreConsts::DICT_ELEMENT_BALANCE] = balance;
    eventMap[CCStoreConsts::DICT_ELEMENT_AMOUNT_ADDED] = amountAdded;

    dispatchCustomEvent(CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED, &eventMap);
}

void CCStoreEventDispatcher::onGoodEquipped(CCEquippableVG* equippableVG) {
    CCStoreInventory::sharedStoreInventory()->refreshOnGoodEquipped(
        equippableVG);

    ValueMap eventMap;
    eventMap[CCStoreConsts::DICT_ELEMENT_EQUIPPABLEVG] =
        equippableVG->toValueMap();
    
    dispatchCustomEvent(CCStoreConsts::EVENT_GOOD_EQUIPPED, &eventMap);
}

void CCStoreEventDispatcher::onGoodUnEquipped(CCEquippableVG* equippableVG) {
    CCStoreInventory::sharedStoreInventory()->refreshOnGoodUnEquipped(
        equippableVG);

    ValueMap eventMap;
    eventMap[CCStoreConsts::DICT_ELEMENT_EQUIPPABLEVG] =
        equippableVG->toValueMap();
    
    dispatchCustomEvent(CCStoreConsts::EVENT_GOOD_UNEQUIPPED, &eventMap);
}

void CCStoreEventDispatcher::onGoodUpgrade(CCVirtualGood* virtualGood,
                                           CCUpgradeVG* upgradeVG) {
    CCStoreInventory::sharedStoreInventory()->refreshOnGoodUpgrade(virtualGood,
                                                                   upgradeVG);

    ValueMap eventMap;
    eventMap[CCStoreConsts::DICT_ELEMENT_GOOD] = virtualGood->toValueMap();
    if (upgradeVG != nullptr) {
        eventMap[CCStoreConsts::DICT_ELEMENT_UPGRADEVG] =
            upgradeVG->toValueMap();
    }
    
    dispatchCustomEvent(CCStoreConsts::EVENT_GOOD_UPGRADE, &eventMap);
}

void CCStoreEventDispatcher::onItemPurchased(
    CCPurchasableVirtualItem* purchasableVirtualItem,
    const std::string& payload) {
    onItemPurchased(purchasableVirtualItem, payload, false);
}

void CCStoreEventDispatcher::onItemPurchased(
    CCPurchasableVirtualItem* purchasableVirtualItem,
    const std::string& payload, bool alsoPush) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();
    eventDict[CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD] = payload;

    dispatchCustomEvent(CCStoreConsts::EVENT_ITEM_PURCHASED, &eventDict);

    if (alsoPush) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ValueMap params;
        params["method"] = "CCStoreEventDispatcher::pushOnItemPurchased";
        params["itemId"] = purchasableVirtualItem->getItemId();
        params["payload"] = payload;
        CCNdkBridge::callNative(params, nullptr);
#endif
    }
}

void CCStoreEventDispatcher::onItemPurchaseStarted(
    CCPurchasableVirtualItem* purchasableVirtualItem) {
    onItemPurchaseStarted(purchasableVirtualItem, false);
}

void CCStoreEventDispatcher::onItemPurchaseStarted(
    CCPurchasableVirtualItem* purchasableVirtualItem, bool alsoPush) {
    ValueMap eventMap;
    eventMap[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] = purchasableVirtualItem->toValueMap();

    dispatchCustomEvent(CCStoreConsts::EVENT_ITEM_PURCHASE_STARTED, &eventMap);

    if (alsoPush) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ValueMap params;
        params["method"] = "CCStoreEventDispatcher::pushOnItemPurchaseStarted";
        params["itemId"] = purchasableVirtualItem->getItemId();
        CCNdkBridge::callNative(params, nullptr);
#endif
    }
}

void CCStoreEventDispatcher::onMarketPurchaseCancelled(
    CCPurchasableVirtualItem* purchasableVirtualItem) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();
    
    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED, &eventDict);
}

void CCStoreEventDispatcher::onMarketPurchaseDeferred(
    CCPurchasableVirtualItem* purchasableVirtualItem,
    const std::string& payload) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();
    eventDict[CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD] = payload;
    
    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_PURCHASE_DEFERRED, &eventDict);
}

void CCStoreEventDispatcher::onMarketPurchase(
    CCPurchasableVirtualItem* purchasableVirtualItem,
    const std::string& payload, const cocos2d::ValueMap& extraInfo) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();
    eventDict[CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD] = payload;
    eventDict[CCStoreConsts::DICT_ELEMENT_EXTRA_INFO] = extraInfo;

    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_PURCHASE, &eventDict);
}

void CCStoreEventDispatcher::onMarketPurchaseStarted(
    CCPurchasableVirtualItem* purchasableVirtualItem) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();
    
    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED, &eventDict);
}

void CCStoreEventDispatcher::onVerificationStarted(
    CCPurchasableVirtualItem* purchasableVirtualItem) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();
    
    dispatchCustomEvent(CCStoreConsts::EVENT_VERIFICATION_STARTED, &eventDict);
}

void CCStoreEventDispatcher::onMarketPurchaseVerification(
    CCPurchasableVirtualItem* purchasableVirtualItem) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();

    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_PURCHASE_VERIFICATION, &eventDict);
}

void CCStoreEventDispatcher::onRestoreTransactionsFinished(bool success) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_SUCCESS] = success;
    
    dispatchCustomEvent(CCStoreConsts::EVENT_RESTORE_TRANSACTION_FINISHED, &eventDict);
}

void CCStoreEventDispatcher::onRestoreTransactionsStarted() {
    dispatchCustomEvent(CCStoreConsts::EVENT_RESTORE_TRANSACTION_STARTED);
}

void CCStoreEventDispatcher::onUnexpectedStoreError(std::int32_t errorCode) {
    onUnexpectedStoreError(errorCode, false);
}

void CCStoreEventDispatcher::onUnexpectedStoreError(std::int32_t errorCode,
                                                    bool alsoPush) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_ERROR_CODE] = errorCode;
    
    dispatchCustomEvent(CCStoreConsts::EVENT_UNEXPECTED_STORE_ERROR, &eventDict);
    
    if (alsoPush) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ValueMap params;
        params["method"] = "CCStoreEventDispatcher::pushOnUnexpectedStoreError";
        params["errorCode"] = errorCode;
        CCNdkBridge::callNative(params, nullptr);
#endif
    }
}

void CCStoreEventDispatcher::onSoomlaStoreInitialized() {
    onSoomlaStoreInitialized(false);
}

void CCStoreEventDispatcher::onSoomlaStoreInitialized(bool alsoPush) {
    dispatchCustomEvent(CCStoreConsts::EVENT_SOOMLA_STORE_INITIALIZED);

    CCStoreInventory::sharedStoreInventory()->refreshLocalInventory();

    if (alsoPush) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) ||                                 \
    (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        ValueMap params;
        params["method"] =
            "CCStoreEventDispatcher::pushOnSoomlaStoreInitialized";
        CCNdkBridge::callNative(params, nullptr);
#endif
    }
}

void CCStoreEventDispatcher::onMarketItemsRefreshed(
    const cocos2d::Vector<CCMarketItem*>& marketItems) {
    ValueVector vvMarketItems;
    for (auto marketItem : marketItems) {
        vvMarketItems.push_back(Value(marketItem->toValueMap()));
    }

    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_MARKET_ITEMS] = vvMarketItems;
    
    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED, &eventDict);
}

void CCStoreEventDispatcher::onMarketItemsRefreshStarted() {
    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_STARTED);
}

void CCStoreEventDispatcher::onMarketItemsRefreshFailed(
    const std::string& errorMessage) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_ERROR_MESSAGE] = errorMessage;
    
    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_FAILED, &eventDict);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void CCStoreEventDispatcher::onMarketRefund(
    CCPurchasableVirtualItem* purchasableVirtualItem) {
    ValueMap eventDict;
    eventDict[CCStoreConsts::DICT_ELEMENT_PURCHASABLE] =
        purchasableVirtualItem->toValueMap();
    
    dispatchCustomEvent(CCStoreConsts::EVENT_MARKET_REFUND, &eventDict);
}

void CCStoreEventDispatcher::onIabServiceStarted() {
    dispatchCustomEvent(CCStoreConsts::EVENT_IAB_SERVICE_STARTED);
}

void CCStoreEventDispatcher::onIabServiceStopped() {
    dispatchCustomEvent(CCStoreConsts::EVENT_IAB_SERVICE_STOPPED);
}
#endif

} // namespace soomla
