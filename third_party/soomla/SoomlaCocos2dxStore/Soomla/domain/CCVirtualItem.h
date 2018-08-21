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

//  Created by Igor Yegoroff on 5/16/13.

#ifndef __CCVirtualItem_H__
#define __CCVirtualItem_H__

#include "cocos2d.h"
#include "CCDomain.h"
#include "CCStoreConsts.h"
#include "CCSoomlaMacros.h"
#include "CCSoomlaEntity.h"
#include "CCError.h"

namespace soomla {
	/** 
     @class CCVirtualItem
     @brief A virtual item.

     This is the parent class of all virtual items in the application.
     Almost every entity in your virtual economy will be a virtual item. There 
     are many types of virtual items - each one of the various types extends
     `CCVirtualItem` and adds its own behavior on top of it.
	*/    
    class CCVirtualItem : public CCSoomlaEntity {
    public:
        const std::string& getItemId();
        
        /**
         Gives your user the given amount of the specific virtual item.
         For example, when your users play your game for the first time you 
         GIVE them 1000 gems.
         
         NOTE: This action is different than `PurchasableVirtualItem<`'s `buy()`:
         You use `give(int amount)`to give your user something for free.
         You use `buy()` to give your user something and get something in return.
         
         @param amount the amount of the specific item to be given.
         @param error A `CCError` for error checking.
         @return The balance after the giving process.
         */
        int give(int amount, CCError **error = NULL) { return give(amount, true, error); }
        
        /**
         Works like "give" but receives an argument, notify, to indicate
         
         @param amount the amount of the specific item to be given
         @param notify Notify of change in user's balance of current virtual item
         @param error A `CCError` for error checking.
         @return The balance after the giving process.
         */
        virtual int give(int amount, bool notify, CCError **error = NULL) = 0;
        
        /**
         Takes from your user the given amount of the specific virtual item.
         For example, when you want to downgrade a virtual good, you take the upgrade.
         
         @param amount The amount of the specific item to be taken.
         @param error A `CCError` for error checking.
         @return The balance after the taking process.
         */
        int take(int amount, CCError **error = NULL) { return take(amount, true, error); }
        
        /**
         Works like "take" but receives an argument, notify, to indicate
         if there has been a change in the balance of the current virtual item.
         
         @param amount The amount of the specific item to be taken.
         @param notify Notify of change in user's balance of current virtual item
         @param error A `CCError` for error checking.
         @return The balance after the taking process.
         */
        virtual int take(int amount, bool notify, CCError **error = NULL) = 0;
        
        /**
         Resets this Virtual Item's balance to the given balance.
         
         @param balance The balance of the current virtual item.
         @param error A `CCError` for error checking.
         @return The balance after the reset process.
         */
        int resetBalance(int balance, CCError **error = NULL) { return resetBalance(balance, true, error); }
        
        /**
         Works like "resetBalance" but receives an argument, notify, to indicate
         if there has been a change in the balance of the current virtual item.
         
         @param balance The balance of the current virtual item.
         @param notify Notify of change in user's balance of current virtual item.
         @param error A `CCError` for error checking.
         @return The balance after the reset process.
         */
        virtual int resetBalance(int balance, bool notify, CCError **error = NULL) = 0;
        
        /**
         Will fetch the balance for the current VirtualItem according to its type.
         
         @param error A `CCError` for error checking.
         @return The balance.
         */
        virtual int getBalance(CCError **error = NULL) = 0;

        /**
         Save this instance with changes that were made to it.
         The saving is done in the metadata in StoreInfo and being persisted to the local DB.
         
         @param saveToDB should the save persist to the local DB.
         */
        virtual void save(bool saveToDB = true);
    };
    
}

#endif /* defined(__CCVirtualItem_H__) */
