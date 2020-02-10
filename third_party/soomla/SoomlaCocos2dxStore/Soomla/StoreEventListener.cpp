//
//  StoreEventListener_.cpp
//  fish
//
//  Created by Zinge on 6/28/16.
//
//

#include "Soomla/StoreEventListener.h"

#include <string>
#include <unordered_map>
#include <vector>

#include "Soomla/Cocos2dxStore.h"

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

#if COCOSCREATOR_VERSION == 2
using EventType = cocos2d::CustomEvent;
#else  // COCOSCREATOR_VERSION == 2
using EventType = cocos2d::EventCustom*; // Note: pointer.
#endif // COCOSCREATOR_VERSION == 2

namespace {
decltype(auto) getUserData(EventType event) {
#if COCOSCREATOR_VERSION == 2
    return *static_cast<cocos2d::ValueMap*>(event.args[0].ptrVal);
#else  // COCOSCREATOR_VERSION == 2
    return *static_cast<cocos2d::ValueMap*>(event->getUserData());
#endif // COCOSCREATOR_VERSION == 2
}

template <class Function, class... Args>
void invokeCallback(Function&& f, Args&&... args) {
    std::invoke(f, std::forward<Args>(args)...);
}

/// All finished events have to use this invoke method.
/// Fix issue: label with black square texts.
template <class Function, class... Args>
void invokeCallbackDelayed(Function&& f, Args&&... args) {
    static int counter;
    static auto target = &counter;
    auto key = "__soomla_dispatch" + std::to_string(counter++);
    cocos2d::Director::getInstance()->getScheduler()->schedule(
        [=](float delta) { //
            std::invoke(f, args...);
        },
        target, 0, 0, 0, false, key);
}
} // namespace

class StoreEventListener::Impl {
public:
    void addListener(const std::string& eventName,
                     const std::function<void(EventType event)>& callback);

    std::unordered_map<std::string, EventListenerProxy> listeners_;
};

void StoreEventListener::Impl::addListener(
    const std::string& eventName,
    const std::function<void(EventType)>& callback) {
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
        [callback](EventType event) {
            auto&& dict = getUserData(event);
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
            invokeCallback(callback, currency->getItemId(), balance,
                           amountAdded);
        });
}

void StoreEventListener::setGoodBalanceChangedCallback(
    const GoodBalanceChangedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
            auto& valueMap =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_GOOD).asValueMap();
            auto itemID =
                valueMap.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID).asString();
            auto balance =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_BALANCE).asInt();
            auto amountAdded =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_AMOUNT_ADDED)
                    .asInt();
            invokeCallback(callback, itemID, balance, amountAdded);
        });
}

void StoreEventListener::setGoodUpgradeCallback(
    const GoodUpgradeCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_GOOD_UPGRADE, [callback](EventType event) {
            auto&& dict = getUserData(event);
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
            invokeCallback(callback, goodID, upgradeID);
        });
}

void StoreEventListener::setItemPurchasedCallback(
    const ItemPurchasedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_ITEM_PURCHASED,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
            auto& purchasable =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_PURCHASABLE)
                    .asValueMap();
            auto itemID =
                purchasable.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID)
                    .asString();
            auto payload =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD)
                    .asString();
            invokeCallback(callback, itemID, payload);
        });
}

void StoreEventListener::setMarketPurchaseStartedCallback(
    const MarketPurchaseStartedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
            auto& valueMap =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_PURCHASABLE)
                    .asValueMap();
            auto itemID =
                valueMap.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID).asString();
            invokeCallback(callback, itemID);
        });
}

void StoreEventListener::setMarketPurchaseCallback(
    const MarketPurchaseCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_PURCHASE,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
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

            invokeCallbackDelayed(callback, itemID, payload, info);
        });
}

void StoreEventListener::setMarketPurchaseCanceledCallback(
    const MarketPurchaseCanceledCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
            auto& valueMap =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_PURCHASABLE)
                    .asValueMap();
            auto itemID =
                valueMap.at(soomla::CCCoreConsts::JSON_ITEM_ITEM_ID).asString();
            invokeCallbackDelayed(callback, itemID);
        });
}

void StoreEventListener::setMarketItemsRefreshStartedCallback(
    const MarketItemsRefreshStartedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_STARTED,
        [callback](EventType event) { //
            invokeCallback(callback);
        });
}

void StoreEventListener::setMarketItemsRefreshedCallback(
    const MarketItemsRefreshedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
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

            invokeCallbackDelayed(callback, marketItemIds);
        });
}

void StoreEventListener::setMarketItemsRefreshFailedCallback(
    const MarketItemsRefreshFailedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_FAILED,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
            auto errorMessage =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_ERROR_MESSAGE)
                    .asString();
            invokeCallbackDelayed(callback, errorMessage);
        });
}

void StoreEventListener::setRestoreTransactionStartedCallback(
    const RestoreTransactionStartedCallback& callback) {
    impl_->addListener(soomla::CCStoreConsts::EVENT_RESTORE_TRANSACTION_STARTED,
                       [callback](EventType event) { //
                           invokeCallback(callback);
                       });
}

void StoreEventListener::setRestoreTransactionFinishedCallback(
    const RestoreTransactionFinishedCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_RESTORE_TRANSACTION_FINISHED,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
            auto succeeded =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_SUCCESS).asBool();
            invokeCallbackDelayed(callback, succeeded);
        });
}

void StoreEventListener::setUnexpectedStoreErrorCallback(
    const UnexpectedStoreErrorCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_UNEXPECTED_STORE_ERROR,
        [callback](EventType event) {
            auto&& dict = getUserData(event);
            auto errorCode =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_ERROR_CODE).asInt();
            invokeCallbackDelayed(callback, errorCode);
        });
}

void StoreEventListener::clear() {
    impl_->listeners_.clear();
}
} // namespace soomla
