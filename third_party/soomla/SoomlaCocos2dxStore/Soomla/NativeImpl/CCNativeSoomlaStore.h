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

#ifndef __CCNativeSoomlaStore_H_
#define __CCNativeSoomlaStore_H_

#include "CCSoomlaStore.h"

namespace soomla {

    /**
     The native implementation of CCSoomlaStore, uses the bridge to talk
     to native implementations of SoomlaStore.
     
     See parent for all functions.
     */
    class CCNativeSoomlaStore : public CCSoomlaStore {
    public:
        virtual void buyMarketItem(const std::string& productId,
                                   const std::string& payload,
                                   CCError** error = NULL);

        virtual void restoreTransactions();
        
        virtual void refreshInventory();
        
        virtual void refreshMarketItemsDetails(CCError **error = NULL);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        virtual bool transactionsAlreadyRestored();
        
#endif
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        virtual void startIabServiceInBg();
        
        virtual void stopIabServiceInBg();
#endif
    protected:
        virtual void loadBillingService();
    };
}

#endif // __CCNativeSoomlaStore_H_
