//
//  Store.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_STORE_HPP_
#define EE_X_STORE_STORE_HPP_

#include <string>

namespace ee {
namespace store {
class StoreAssets;

class Store {
public:
    static Store& getInstance();

    static void initialize(const StoreAssets& assets);

    virtual void buyMarketItem(const std::string& productId,
                               const std::string& payload);

    /// Restores this user's previous transactions.
    virtual void restoreTransactions();

    /// Creates a list of all metadata stored in the market (the items that have
    /// been purchased).
    virtual void refreshInventory();

    /// Refreshes the details of all market-purchasable items that were defined
    /// in the market.
    virtual void refreshMarketItemDetails();

protected:
    Store() = default;
    virtual ~Store() = default;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_STORE_HPP_ */
