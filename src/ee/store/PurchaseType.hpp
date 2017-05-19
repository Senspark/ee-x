//
//  PurchaseType.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_PURCHASE_TYPE_HPP_
#define EE_X_STORE_PURCHASE_TYPE_HPP_

#include <string>

namespace ee {
namespace store {
/// Specializes the purchase type of a virtual item.
/// There are two purchase types:
/// - Purchase with market.
/// - Purchase with other virtual items.
class PurchaseType {
public:
    virtual ~PurchaseType();

    virtual void buy(const std::string& payload) = 0;

    /// Checks if it is enough funds to afford this item.
    /// @return True if it is enough funds to afford the item, false otherwise.
    virtual bool canAfford() = 0;

    const std::string& getAssociatedItemId() const;

    void setAssociatedItemId(const std::string& itemId);

protected:
    PurchaseType();

    virtual bool init();

private:
    std::string associatedItemId_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASE_TYPE_HPP_ */
