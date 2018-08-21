
#ifndef __CCStoreEventDispatcher_H_
#define __CCStoreEventDispatcher_H_

#include "CCEquippableVG.h"
#include "CCPurchasableVirtualItem.h"
#include "CCUpgradeVG.h"
#include "CCVirtualCurrency.h"
#include "CCVirtualGood.h"
#include "CCMarketItem.h"
#include "cocos2d.h"

namespace soomla {
class CCStoreEventDispatcher : public cocos2d::Ref {
public:
    /**
    This class is singleton, access it with this function.
    */
    static CCStoreEventDispatcher* getInstance();

    bool init();

    /**
     Fired when SOOMLA knows that billing is NOT supported on the device.

     Event Name - CCStoreConsts::EVENT_BILLING_NOT_SUPPORTED
     */
    virtual void onBillingNotSupported();

    /**
     Fired when SOOMLA knows that billing IS supported on the device.

     Event Name - CCStoreConsts::EVENT_BILLING_SUPPORTED
     */
    virtual void onBillingSupported();

    /**
     Fired when the balance of a specidifc virtual currency has changed.

     Event Name - CCStoreConsts::EVENT_CURRENCY_BALANCE_CHANGED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_CURRENCY - CCVirtualCurrency - The currency
     whose balance has changed.
     CCStoreConsts::DICT_ELEMENT_BALANCE - __Integer - The balance of the
     currency.
     CCStoreConsts::DICT_ELEMENT_AMOUNT_ADDED - __Integer - The amount added
     to the currency.
     */
    virtual void onCurrencyBalanceChanged(CCVirtualCurrency* virtualCurrency,
                                          int balance, int amountAdded);

    /**
     Fired when the balance of a specific virtual good has changed.

     Event Name - CCStoreConsts::EVENT_GOOD_BALANCE_CHANGED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_GOOD - CCVirtualGood - The good whose
     balance has changed.
     CCStoreConsts::DICT_ELEMENT_BALANCE - __Integer - The balance of the
     good.
     CCStoreConsts::DICT_ELEMENT_AMOUNT_ADDED - __Integer - The amount added
     to the good.
     */
    virtual void onGoodBalanceChanged(CCVirtualGood* virtualGood, int balance,
                                      int amountAdded);

    /**
     Fired when a specific equippable virtual good has been equipped.

     Event Name - CCStoreConsts::EVENT_GOOD_EQUIPPED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_EQUIPPABLEVG - CCEquippableVG - The good
     that is being equipped.
     */
    virtual void onGoodEquipped(CCEquippableVG* equippableVG);

    /**
     Fired when a specific equippable virtual good has been equipped.

     Event Name - CCStoreConsts::EVENT_GOOD_UNEQUIPPED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_EQUIPPABLEVG - CCEquippableVG - The good
     that is being unequipped.
     */
    virtual void onGoodUnEquipped(CCEquippableVG* equippableVG);

    /**
     Fired when a specific virtual good has been upgraded.

     Event Name - CCStoreConsts::EVENT_GOOD_UPGRADE
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_GOOD - CCVirtualGood - The virtual good
     that is being upgraded.
     CCStoreConsts::DICT_ELEMENT_UPGRADEVG - CCUpgradeVG - The upgrade.
     */
    virtual void onGoodUpgrade(CCVirtualGood* virtualGood,
                               CCUpgradeVG* upgradeVG);

    /**
     Fired when when a specific virtual good has been purchased.

     Event Name - CCStoreConsts::EVENT_ITEM_PURCHASED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     The item being purchased.
     CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD - __String - The developer
     payload.
     */
    virtual void
    onItemPurchased(CCPurchasableVirtualItem* purchasableVirtualItem,
                    const std::string& payload);

    virtual void
    onItemPurchased(CCPurchasableVirtualItem* purchasableVirtualItem,
                    const std::string& payload, bool alsoPush);

    /**
     Fired when a purchase process has started.

     Event Name - CCStoreConsts::EVENT_ITEM_PURCHASE_STARTED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     The item whose purchase has started.
     */
    virtual void
    onItemPurchaseStarted(CCPurchasableVirtualItem* purchasableVirtualItem);

    virtual void
    onItemPurchaseStarted(CCPurchasableVirtualItem* purchasableVirtualItem,
                          bool alsoPush);

    /**
     Fired when a market (App Store, Google Play, etc..) purchase has been
     cancelled.

     Event Name - CCStoreConsts::EVENT_MARKET_PURCHASE_CANCELED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     the item whose purchase is being cancelled.
     */
    virtual void
    onMarketPurchaseCancelled(CCPurchasableVirtualItem* purchasableVirtualItem);

    /**
     Fired when a market (App Store, Google Play, etc..) purchase has been
     deferred (iOS only).

     Event Name - CCStoreConsts::EVENT_MARKET_PURCHASE_DEFERRED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     the item whose purchase is being deferred.
     */
    virtual void
    onMarketPurchaseDeferred(CCPurchasableVirtualItem* purchasableVirtualItem,
                             const std::string& payload);

    /**
     Fired when a market item from the store (App Store, Google Play, etc..)
     has been purchased.

     Event Name - CCStoreConsts::EVENT_MARKET_PURCHASE
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem - The
     market item being purchased. CCStoreConsts::DICT_ELEMENT_DEVELOPERPAYLOAD -
     __String - The developer payload. CCStoreConsts::DICT_ELEMENT_EXTRA_INFO -
     contains platform specific information about the market purchase Android:
     The "extra" dictionary will contain: 'token', 'orderId', 'originalJson',
     'signature', 'userId' iOS: The "extra" dictionary will contain:
     'receiptUrl', 'transactionIdentifier', 'receiptBase64', 'transactionDate',
     'originalTransactionDate', 'originalTransactionIdentifier'
    */
    virtual void
    onMarketPurchase(CCPurchasableVirtualItem* purchasableVirtualItem,
                     const std::string& payload,
                     const cocos2d::ValueMap& extraInfo);

    /**
     Fired when a purchase process has started, where the item is being
     purchased from the store (App Store, Google Play, etc..).

     Event Name - CCStoreConsts::EVENT_MARKET_PURCHASE_STARTED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     The market item whose purchase has started.
     */
    virtual void
    onMarketPurchaseStarted(CCPurchasableVirtualItem* purchasableVirtualItem);

    /**
     Fired when a market purchase verification process has started.

     Event Name - CCStoreConsts::EVENT_VERIFICATION_STARTED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     The market item whose purchase is being verified.
     */
    virtual void
    onVerificationStarted(CCPurchasableVirtualItem* purchasableVirtualItem);

    /**
     Fired when a market purchase verification process has finished.

     Event Name - CCStoreConsts::EVENT_MARKET_PURCHASE_VERIFICATION
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     The market item whose purchase is being verified.
     */
    virtual void onMarketPurchaseVerification(
        CCPurchasableVirtualItem* purchasableVirtualItem);

    /**
     Fired when a restore transactions process has finished.

     Event Name - CCStoreConsts::EVENT_RESTORE_TRANSACTION_FINISHED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_SUCCESS - __Bool - Indicates if the
     restore transactions process finished successfully.
     */
    virtual void onRestoreTransactionsFinished(bool success);

    /**
     Fired when a restore transactions process has started.

     Event Name - CCStoreConsts::EVENT_RESTORE_TRANSACTION_STARTED
     */
    virtual void onRestoreTransactionsStarted();

    /**
     Fired when an unexpected error occurs in the store.

     Event Name - CCStoreConsts::EVENT_UNEXPECTED_STORE_ERROR
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_ERROR_CODE - __Integer -
     The description of the error which occured.
     */
    virtual void onUnexpectedStoreError(std::int32_t errorCode);

    virtual void onUnexpectedStoreError(std::int32_t errorCode,
                                        bool alsoPush);

    /**
     Fired when store controller is initialized.

     Event Name - CCStoreConsts::EVENT_SOOMLA_STORE_INITIALIZED
     */
    virtual void onSoomlaStoreInitialized();

    virtual void onSoomlaStoreInitialized(bool alsoPush);

    /**
     Fired when a market item refreshed process has completed.

     Event Name - CCStoreConsts::EVENT_MARKET_ITEMS_REFRESHED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_MARKET_ITEMS - __Array -
     The market items which were refreshed.
     */
    virtual void onMarketItemsRefreshed(const cocos2d::Vector<CCMarketItem*>& marketItems);

/** Android only events*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    /**
     Fired when a market item is being refunded.

     Event Name - CCStoreConsts::EVENT_MARKET_REFUND
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     The item that is being refunded in the market.
     */
    virtual void
    onMarketRefund(CCPurchasableVirtualItem* purchasableVirtualItem);

    /**
     Fired when a restore transactions process has finished.

     Event Name - CCStoreConsts::EVENT_MARKET_PURCHASE_VERIFICATION
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_PURCHASABLE - CCPurchasableVirtualItem -
     The market item whose purchase is being verified.
     */
    virtual void onIabServiceStarted();

    /**
     Fired when in-app billing service is started.

     Event Name - CCStoreConsts::EVENT_IAB_SERVICE_STARTED
     */
    virtual void onIabServiceStopped();
#endif

    /**
     Fired when in-app billing service is stopped.

     Event Name - CCStoreConsts::EVENT_IAB_SERVICE_STOPPED
     */
    virtual void onMarketItemsRefreshStarted();

    /**
     Fired when a market item refreshed process has failed.

     Event Name - CCStoreConsts::EVENT_MARKET_ITEMS_REFRESH_FAILED
     Event Data (__Dictionary):
     CCStoreConsts::DICT_ELEMENT_ERROR_MESSAGE - __String - The error which
     caused the failure.
     */
    virtual void onMarketItemsRefreshFailed(const std::string& errorMessage);
};
} // namespace soomla

#endif //__CCStoreEventDispatcher_H_
