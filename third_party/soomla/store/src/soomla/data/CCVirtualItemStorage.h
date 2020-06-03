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

#ifndef __CCVirtualItemStorage_H__
#define __CCVirtualItemStorage_H__

#ifdef __cplusplus

#include <cocos2d.h>

#include <soomla/CCError.h>

#include <soomla/domain/CCVirtualItem.h>

namespace soomla {
/**
 This class is an abstract definition of a Virtual Item Storage.
 */
class CCVirtualItemStorage : public cocos2d::Ref {
public:
    /**
     Retrieves the balance of the given virtual item.
     @param item The required virtual item.
     @param error Gets A `CCError` for error checking.
     @return The balance of the required virtual item.
     */
    virtual int getBalance(CCVirtualItem* item, CCError** error = nullptr);

    /**
     Sets the balance of the given virtual item to be the given balance, and
     if notify is true posts the change in the balance to the event bus.
     @param item the required virtual item.
     @param balance the new balance to be set.
     @param error Gets A `CCError` for error checking.
     @return the balance of the required virtual item
     */
    virtual int setBalance(CCVirtualItem* item, int balance,
                           CCError** error = nullptr) {
        return setBalance(item, balance, true, error);
    }
    /**
     Same as the other `setBalance` but with "notify".
     @param item the required virtual item.
     @param balance the new balance to be set.
     @param notify if notify is true post balance change event.
     @param error Gets A `CCError` for error checking.
     @return the balance of the required virtual item
     */
    virtual int setBalance(CCVirtualItem* item, int balance, bool notify,
                           CCError** error = nullptr);

    /**
     Adds the given amount of items to the storage.
     @param item the required virtual item.
     @param amount the amount of items to add.
     @param error Gets A `CCError` for error checking.
     @return the balance of the required virtual item
     */
    virtual int add(CCVirtualItem* item, int amount, CCError** error = nullptr) {
        return add(item, amount, true, error);
    }
    /**
     Adds the given amount of items to the storage, and if notify is true
     posts the change in the balance to the event bus.
     @param item the required virtual item.
     @param amount the amount of items to add.
     @param notify if notify is true post balance change event.
     @param error Gets A `CCError` for error checking.
     @return the balance of the required virtual item
     */
    virtual int add(CCVirtualItem* item, int amount, bool notify,
                    CCError** error = nullptr);

    /**
     Removes the given amount from the given virtual item's balance.
     @param item the required virtual item.
     @param amount the amount of items to remove.
     @param error Gets A `CCError` for error checking.
     @return the balance of the required virtual item
     */
    virtual int remove(CCVirtualItem* item, int amount,
                       CCError** error = nullptr) {
        return remove(item, amount, true, error);
    }
    /**
     Removes the given amount of items from the storage, and if notify is true
     posts the change in the balance to the event bus.
     @param item the required virtual item.
     @param amount the amount of items to remove.
     @param notify if notify is true post balance change event.
     @param error Gets A `CCError` for error checking.
     @return the balance of the required virtual item
     */
    virtual int remove(CCVirtualItem* item, int amount, bool notify,
                       CCError** error = nullptr);

protected:
    virtual std::string keyBalance(const std::string& itemId) const = 0;
    virtual void postBalanceChangeEvent(CCVirtualItem* item, int balance,
                                        int amountAdded) = 0;
};
} // namespace soomla

#endif // __cplusplus

#endif // __CCVirtualItemStorage_H__
