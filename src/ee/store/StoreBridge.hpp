//
//  Store.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_STORE_BRIDGE_HPP_
#define EE_X_STORE_STORE_BRIDGE_HPP_

#include <string>
#include <vector>

namespace ee {
namespace store {
class StoreAssets;

class Store {
public:
    bool canPurchase() const;

    void purchase(const std::string& productId);

    void initialize(const StoreAssets& assets);

    void buyMarketItem(const std::string& productId,
                       const std::string& payload);

    /// Restores this user's previous transactions.
    void restoreTransactions();

    void requestProducts(const std::vector<std::string>& productIds);

    /// Creates a list of all metadata stored in the market (the items that have
    /// been purchased).
    void refreshInventory();

    /// Refreshes the details of all market-purchasable items that were defined
    /// in the market.
    void refreshMarketItemDetails();

protected:
    Store();
    ~Store();

private:
    void onRequestProductSucceeded();

    void onRequestProductFailed(int errorCode);

    void onRestorePurchasesSucceeded();

    void onRestorePurchasesFailed(int errorCode);

    void onTransactionSucceeded(const std::string& productId,
                                const std::string& transactionId);

    void onTransactionFailed(const std::string& productId, int errorCode);

    void onTransactionRestored(const std::string& productId,
                               const std::string& transactionId);
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_STORE_BRIDGE_HPP_ */
