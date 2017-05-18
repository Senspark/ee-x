//
//  PurchasableVirtualItem.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_PURCHASABLE_VIRTUAL_ITEM_HPP_
#define EE_X_STORE_PURCHASABLE_VIRTUAL_ITEM_HPP_

#include <memory>

#include "ee/store/VirtualItem.hpp"

namespace ee {
namespace store {
class PurchaseType;

class PurchasableVirtualItem : public VirtualItem {
public:
    const std::unique_ptr<PurchaseType>& getPurchaseType() const;

    /// Checks if it is enough funds to afford this item.
    bool canAfford();

    void buy(const std::string& payload);

    /// Determines if the user is in a state that allows him/her to buy this
    /// item.
    /// @return Whether the user can buy this item.
    virtual bool canBuy() = 0;

private:
    std::unique_ptr<PurchaseType> purchaseType_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASABLE_VIRTUAL_ITEM_HPP_ */
