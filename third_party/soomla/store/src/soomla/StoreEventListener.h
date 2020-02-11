//
//  StoreEventListener_.hpp
//  fish
//
//  Created by Zinge on 6/28/16.
//
//

#ifndef SOOMLA_STORE_STORE_EVENT_LISTENER_HPP_
#define SOOMLA_STORE_STORE_EVENT_LISTENER_HPP_

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

namespace soomla {
class StoreEventListener {
private:
    using BalanceChangedCallback = std::function<void(
        const std::string& itemId, int balance, int amountAdded)>;

    using PurchasableIdCallback =
        std::function<void(const std::string& purchasableId)>;

public:
    using CurrencyBalanceChangedCallback = BalanceChangedCallback;

    using GoodBalanceChangedCallback = BalanceChangedCallback;

    using GoodUpgradeCallback = std::function<void(
        const std::string& goodId, const std::string& upgradeId)>;

    using ItemPurchasedCallback = std::function<void(
        const std::string& purchasableId, const std::string& payload)>;

    using MarketPurchaseStartedCallback = PurchasableIdCallback;

    using MarketPurchaseCallback = std::function<void(
        const std::string& purchasableId, const std::string& payload,
        const std::unordered_map<std::string, std::string>& extraInfo)>;

    using MarketPurchaseCanceledCallback = PurchasableIdCallback;

    using MarketItemsRefreshStartedCallback = std::function<void()>;

    using MarketItemsRefreshedCallback =
        std::function<void(const std::vector<std::string>& marketItemIds)>;

    using MarketItemsRefreshFailedCallback =
        std::function<void(const std::string& errorMessage)>;

    using RestoreTransactionStartedCallback = std::function<void()>;

    using RestoreTransactionFinishedCallback =
        std::function<void(bool succeeded)>;

    using UnexpectedStoreErrorCallback = std::function<void(int errorCode)>;

public:
    StoreEventListener();
    virtual ~StoreEventListener();

    void setCurrencyBalanceChangedCallback(
        const CurrencyBalanceChangedCallback& callback);

    void
    setGoodBalanceChangedCallback(const GoodBalanceChangedCallback& callback);

    void setGoodUpgradeCallback(const GoodUpgradeCallback& callback);

    void setItemPurchasedCallback(const ItemPurchasedCallback& callback);

    void setMarketPurchaseStartedCallback(
        const MarketPurchaseStartedCallback& callback);

    void setMarketPurchaseCallback(const MarketPurchaseCallback& callback);

    void setMarketPurchaseCanceledCallback(
        const MarketPurchaseCanceledCallback& callback);

    void setMarketItemsRefreshStartedCallback(
        const MarketItemsRefreshStartedCallback& callback);

    void setMarketItemsRefreshedCallback(
        const MarketItemsRefreshedCallback& callback);

    void setMarketItemsRefreshFailedCallback(
        const MarketItemsRefreshFailedCallback& callback);

    void setRestoreTransactionStartedCallback(
        const RestoreTransactionStartedCallback& callback);

    void setRestoreTransactionFinishedCallback(
        const RestoreTransactionFinishedCallback& callback);

    void setUnexpectedStoreErrorCallback(
        const UnexpectedStoreErrorCallback& callback);

    void clear();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};
} // namespace soomla

#endif /* SOOMLA_STORE_STORE_EVENT_LISTENER_HPP_ */
