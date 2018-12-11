//
//  StoreEventListener_.cpp
//  fish
//
//  Created by Zinge on 6/28/16.
//
//

#include <string>
#include <unordered_map>
#include <vector>

#include "StoreEventListener.h"

#include <Cocos2dxStore.h>

#if __has_include(<base/CCDirector.h>)
#define COCOSCREATOR_VERSION 1
#else
#define COCOSCREATOR_VERSION 2
#endif

#if COCOSCREATOR_VERSION == 2
#include "cocos/scripting/js-bindings/event/EventDispatcher.h"
#endif

namespace soomla {
class EventListenerProxy final {
public:
#if COCOSCREATOR_VERSION == 2
    EventListenerProxy(
        const std::string& eventName,
        const std::function<void(cocos2d::CustomEvent event)>& callback) {
        wrapped_ = cocos2d::EventDispatcher::addCustomEventListener(eventName,
                                                                    callback);
        eventName_ = eventName;
    }
    ~EventListenerProxy() {
        cocos2d::EventDispatcher::removeCustomEventListener(eventName_,
                                                            wrapped_);
    }
#elif COCOSCREATOR_VERSION == 1
    EventListenerProxy(
        const std::string& eventName,
        const std::function<void(cocos2d::EventCustom* event)>& callback) {
        wrapped_ = cocos2d::Director::getInstance()
                       ->getEventDispatcher()
                       ->addCustomEventListener(eventName, callback);
    }

    ~EventListenerProxy() {
        cocos2d::Director::getInstance()
            ->getEventDispatcher()
            ->removeEventListener(wrapped_);
    }
#endif

    EventListenerProxy(const EventListenerProxy&) = delete;
    EventListenerProxy& operator=(const EventListenerProxy&) = delete;

private:
#if COCOSCREATOR_VERSION == 2
    uint32_t wrapped_;
    std::string eventName_;
#elif COCOSCREATOR_VERSION == 1
    cocos2d::EventListener* wrapped_;
#endif
};

class StoreEventListener::Impl {
public:
    void addListener(
        const std::string& eventName,
#if COCOSCREATOR_VERSION == 2
        const std::function<void(cocos2d::CustomEvent event)>& callback);
#elif COCOSCREATOR_VERSION == 1
        const std::function<void(cocos2d::EventCustom* event)>& callback);
#endif
    std::unordered_map<std::string, EventListenerProxy> listeners_;
};

void StoreEventListener::Impl::addListener(
    const std::string& eventName,
#if COCOSCREATOR_VERSION == 2
    const std::function<void(cocos2d::CustomEvent)>& callback) {
#elif COCOSCREATOR_VERSION == 1
    const std::function<void(cocos2d::EventCustom*)>& callback) {
#endif
    bool inserted =
        listeners_
            .emplace(std::piecewise_construct, std::forward_as_tuple(eventName),
                     std::forward_as_tuple(eventName, callback))
            .second;
    CC_ASSERT(inserted);
}

StoreEventListener::StoreEventListener()
    : impl_(new Impl()) {}

StoreEventListener::~StoreEventListener() = default;

void StoreEventListener::setCurrencyBalanceChangedCallback(
    const CurrencyBalanceChangedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& valueMap =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_CURRENCY)
                    .asValueMap();
            auto currency =
                soomla::CCVirtualCurrency::createWithValueMap(valueMap);
            auto balance = dict.at(soomla::CCStoreConsts::CCStoreConsts::
                                       DICT_ELEMENT_BALANCE)
                               .asInt();
            auto amountAdded =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_AMOUNT_ADDED)
                    .asInt();

            callback(currency->getItemId(), balance, amountAdded);
        });
}

void StoreEventListener::setGoodBalanceChangedCallback(
    const GoodBalanceChangedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& valueMap =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_GOOD).asValueMap();
            auto itemID =
                valueMap.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID).asString();
            auto balance =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_BALANCE).asInt();
            auto amountAdded =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_AMOUNT_ADDED)
                    .asInt();
            callback(itemID, balance, amountAdded);
        });
}

void StoreEventListener::setGoodUpgradeCallback(
    const GoodUpgradeCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_GOOD_UPGRADE,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& vmGood =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_GOOD).asValueMap();
            auto& vmUpgrade =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_UPGRADEVG)
                    .asValueMap();
            auto goodID =
                vmGood.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID).asString();
            auto upgradeID =
                vmUpgrade.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID)
                    .asString();
            callback(goodID, upgradeID);
        });
}

void StoreEventListener::setItemPurchasedCallback(
    const ItemPurchasedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_ITEM_PURCHASED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& purchasable =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_PURCHASABLE)
                    .asValueMap();
            auto itemID =
                purchasable.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID)
                    .asString();
            auto payload =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD)
                    .asString();
            callback(itemID, payload);
        });
}

void StoreEventListener::setMarketPurchaseStartedCallback(
    const MarketPurchaseStartedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& valueMap =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_PURCHASABLE)
                    .asValueMap();
            auto itemID =
                valueMap.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID).asString();
            callback(itemID);
        });
}

void StoreEventListener::setMarketPurchaseCallback(
    const MarketPurchaseCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_PURCHASE,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& purchasable =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_PURCHASABLE)
                    .asValueMap();
            auto payload =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD)
                    .asString();
            auto& extraInfo =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_EXTRA_INFO)
                    .asValueMap();

            auto itemID =
                purchasable.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID)
                    .asString();

            std::unordered_map<std::string, std::string> info;
            for (auto& elm : extraInfo) {
                info.emplace(elm.first, elm.second.asString());
            }

            callback(itemID, payload, info);
        });
}

void StoreEventListener::setMarketPurchaseCanceledCallback(
    const MarketPurchaseCanceledCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& valueMap =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_PURCHASABLE)
                    .asValueMap();
            auto itemID =
                valueMap.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID).asString();
            callback(itemID);
        });
}

void StoreEventListener::setMarketItemsRefreshStartedCallback(
    const MarketItemsRefreshStartedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_STARTED,
        std::bind(callback));
}

void StoreEventListener::setMarketItemsRefreshedCallback(
    const MarketItemsRefreshedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto& array =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_MARKET_ITEMS)
                    .asValueVector();

            std::vector<std::string> marketItemIds;
            marketItemIds.reserve(static_cast<std::size_t>(array.size()));

            for (auto&& elt : array) {
                auto valueMap = elt.asValueMap();
                auto productID =
                    valueMap
                        .at(soomla::CCStoreConsts::JSON_MARKET_ITEM_PRODUCT_ID)
                        .asString();
                marketItemIds.emplace_back(productID);
            }

            callback(marketItemIds);
        });
}

void StoreEventListener::setMarketItemsRefreshFailedCallback(
    const MarketItemsRefreshFailedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_FAILED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto errorMessage =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_ERROR_MESSAGE)
                    .asString();
            callback(errorMessage);
        });
}

void StoreEventListener::setRestoreTransactionStartedCallback(
    const RestoreTransactionStartedCallback& callback) {
    impl_->addListener(soomla::CCStoreConsts::EVENT_RESTORE_TRANSACTION_STARTED,
                       std::bind(callback));
}

void StoreEventListener::setRestoreTransactionFinishedCallback(
    const RestoreTransactionFinishedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_RESTORE_TRANSACTION_FINISHED,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto succeeded =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_SUCCESS).asBool();
            callback(succeeded);
        });
}

void StoreEventListener::setUnexpectedStoreErrorCallback(
    const UnexpectedStoreErrorCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_UNEXPECTED_STORE_ERROR,
#if COCOSCREATOR_VERSION == 2
        [callback](cocos2d::CustomEvent event)
#elif COCOSCREATOR_VERSION == 1
        [callback](cocos2d::EventCustom* event)
#endif
        {
            auto&& dict =
#if COCOSCREATOR_VERSION == 2
                *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#elif COCOSCREATOR_VERSION == 1
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif
            auto errorCode =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_ERROR_CODE).asInt();
            callback(errorCode);
        });
}

void StoreEventListener::clear() {
    impl_->listeners_.clear();
}
} // namespace soomla
