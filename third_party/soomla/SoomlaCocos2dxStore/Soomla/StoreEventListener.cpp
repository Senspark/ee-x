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

namespace soomla {
class EventListenerProxy final {
public:
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

    EventListenerProxy(const EventListenerProxy&) = delete;
    EventListenerProxy& operator=(const EventListenerProxy&) = delete;

private:
    cocos2d::EventListener* wrapped_;
};

class StoreEventListener::Impl {
public:
    void addListener(
        const std::string& eventName,
        const std::function<void(cocos2d::EventCustom* event)>& callback);

    std::unordered_map<std::string, EventListenerProxy> listeners_;
};

void StoreEventListener::Impl::addListener(
    const std::string& eventName,
    const std::function<void(cocos2d::EventCustom*)>& callback) {
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
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
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
            auto succeeded =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_SUCCESS).asBool();
            callback(succeeded);
        });
}

void StoreEventListener::setUnexpectedStoreErrorCallback(
    const UnexpectedStoreErrorCallback& callback) {
    impl_->addListener(
        soomla::CCStoreConsts::EVENT_UNEXPECTED_STORE_ERROR,
        [callback](cocos2d::EventCustom* event) {
            auto&& dict =
                *static_cast<cocos2d::ValueMap*>(event->getUserData());
            auto errorCode =
                dict.at(soomla::CCStoreConsts::DICT_ELEMENT_ERROR_CODE).asInt();
            callback(errorCode);
        });
}

void StoreEventListener::clear() {
    impl_->listeners_.clear();
}
} // namespace soomla
