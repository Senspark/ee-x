//
//  PurchaseWithVirtualItem.hpp
//  ee_x
//
//  Created by Zinge on 5/18/17.
//
//

#ifndef EE_X_STORE_PURCHASE_WITH_VIRTUAL_ITEM_HPP_
#define EE_X_STORE_PURCHASE_WITH_VIRTUAL_ITEM_HPP_

#include <memory>

#include "ee/store/PurchaseType.hpp"

namespace ee {
namespace store {
class PurchaseWithVirtualItem : public PurchaseType {
public:
    virtual ~PurchaseWithVirtualItem();

    /// Creates a `PurchaseWithVirtualItem`.
    /// @param itemId The item ID of the virtual item that is used to pay with
    /// in order to make the purchase.
    /// @param amount The amount of the payment item that is needed in order to
    /// make the purchase.
    /// @return The purchase with virtual item type.
    static std::unique_ptr<PurchaseWithVirtualItem>
    create(const std::string& itemId, int amount);

    virtual void buy(const std::string& payload) override;

    virtual bool canAfford() override;

protected:
    PurchaseWithVirtualItem();
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASE_WITH_VIRTUAL_ITEM_HPP_ */
