//
//  VirtualItemStorage.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_VIRTUAL_ITEM_STORAGE_HPP_
#define EE_X_STORE_VIRTUAL_ITEM_STORAGE_HPP_

namespace ee {
namespace store {
class VirtualItem;

class VirtualItemStorage {
public:
    virtual ~VirtualItemStorage() = default;

    /// Retrieves the balance of the given virtual item.
    /// @param item The required virtual item.
    /// @return The balance of the required virtuam item.
    virtual int getBalance(VirtualItem* item);

    /// Sets the balance of the given virtual item to be the given balance, and
    /// if notify is true posts the change in the balance to the event bus.
    /// @param item The required virtual item.
    /// @param balance the new balance to be set.
    /// @param notify If notify is true posts balance change event.
    /// @return The balance of the required virtual item.
    virtual int setBalance(VirtualItem* item, int balance, bool notify);

    /// Adds the given amount of items to the storage, and if notify is true
    /// posts the change in the balance to the event bus.
    /// @param item The required virtual item.
    /// @param amount The amount of items to add.
    /// @param notify If notify is true posts balance change event.
    /// @return The balance of the required virtual item.
    virtual int add(VirtualItem* item, int amount, bool notify);

    /// Removes the given amount of items from the storage, and if notify is
    /// true posts the change in the balance to the event bus.
    /// @param item The required virtual item.
    /// @param amount The amount of items to add.
    /// @param notify If notify is true posts balance change event.
    /// @return The balance of the required virtual item.
    virtual int remove(VirtualItem* item, int amount, bool notify);
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_VIRTUAL_ITEM_STORAGE_HPP_ */
