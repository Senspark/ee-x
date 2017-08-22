//
//  VirtualItem.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_VIRTUAL_ITEM_HPP_
#define EE_X_STORE_VIRTUAL_ITEM_HPP_

#include <string>

namespace ee {
namespace store {
/// This is the base class of all virtual items.
class VirtualItem {
public:
    virtual ~VirtualItem();

    const std::string& getItemId() const;

    /// Fetches the balance of this item.
    /// @return The balance.
    virtual int getBalance() = 0;

    /// Resets this item's balance to the given balance.
    /// @param balance The balance to reset to.
    /// @param notify Whether or not to notify of change in user's balance.
    /// @param The balance after the reset process.
    virtual int setBalance(int balance, bool notify) = 0;

    /// Changes the amount of this item.
    /// @param amount The amount of this item.
    /// @param notify Whether or not to notify of change in user's balance.
    /// @return The balance after the giving process.
    virtual int addBalance(int amount, bool notify) = 0;

    virtual void save(bool saveToDb = true);

protected:
    VirtualItem();
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_VIRTUAL_ITEM_HPP_ */
