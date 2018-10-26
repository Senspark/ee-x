/*
 Copyright (C) 2012-2014 Soomla Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef __CCSoomlaStore_H_
#define __CCSoomlaStore_H_

#include "CCError.h"
#include "CCStoreAssets.h"
#include "cocos2d.h"

namespace soomla {
/**
 @class CCSoomlaStore
 @brief An interface to the native SoomlaStore class.

 An interface to the native StoreController class, use this class to
 access the native StoreController functionality. This class holds the most
 basic assets needed to operate the Store.
 */
class CCSoomlaStore : public cocos2d::Ref {
public:
    /**
     This class is singleton, use this function to access it.
    */
    static CCSoomlaStore* getInstance();

    /**
     Initializes the singleton instance of CCSoomlaStore
     @param storeAssets The game's economy
     @param storeParams The initialization parameters
     This dictionary can contain the following:
     "androidPublicKey": __String - Your Android public key.
     "testPurchases": __Bool - Should test purchases be used in Android.
     "SSV": __Bool - Whether or not to enable server side
     verification of purchases.
     */
    static void initialize(CCStoreAssets* storeAssets,
                           const cocos2d::ValueMap& storeParams);

    /**
     Starts an in app purchase process in the market (App Store, Google
     Play, etc...).
     @param productId The product ID of the item in the market (App Store,
            Google Play, etc..).
     @param error A `CCError` for error checking.
     */
    virtual void buyMarketItem(const std::string& productId,
                               const std::string& payload,
                               CCError** error = nullptr);
    /**
     Restores this user's previous transactions.
     */
    virtual void restoreTransactions() {}

    /**
     Creates a list of all metadata stored in the Market (the items that
     have been purchased). The metadata includes the item's name,
     description, price, product id, etc..
     */
    virtual void refreshInventory() {}

    /**
     Refreshes the details of all market-purchasable items that were defined
     in the market (App Store, Google Play, etc..).
     @param error A `CCError` for error checking.
     */
    virtual void refreshMarketItemsDetails(CCError** error = nullptr) {}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    /**
     Checks if the user has already restored his/her transactions.
     @return Whether or not the user restored his/her transactions.
    */
    virtual bool transactionsAlreadyRestored() { return true; }

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    /**
     Starts the in-app billing service in background.
     */
    virtual void startIabServiceInBg() {}

    /**
     Stops the in-app billing service in background.
     */
    virtual void stopIabServiceInBg() {}
#endif
protected:
    /**
     Loads the billing service for the store
     */
    virtual void loadBillingService() {}

private:
    static bool initialized;
};
}; // namespace soomla

#endif // !__CCSoomlaStore_H_
