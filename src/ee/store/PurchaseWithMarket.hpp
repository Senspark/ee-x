//
//  PurchaseWithMarket.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_PURCHASE_WITH_MARKET_HPP_
#define EE_X_STORE_PURCHASE_WITH_MARKET_HPP_

#include <memory>

#include "ee/store/PurchaseType.hpp"

namespace ee {
namespace store {
class PurchaseWithMarket : public PurchaseType {
public:
    PurchaseWithMarket();

    virtual ~PurchaseWithMarket();

    /// Creates a `PurchaseWithMarket`.
    /// @param productId The product id of the item in the native store.
    /// @return The purchase with market type.
    static std::unique_ptr<PurchaseWithMarket>
    create(const std::string& productId);

    virtual void buy(const std::string& payload) override;

    virtual bool canAfford() override;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASE_WITH_MARKET_HPP_ */
