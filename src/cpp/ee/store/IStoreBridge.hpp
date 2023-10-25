#ifndef EE_X_I_STORE_BRIDGE_HPP
#define EE_X_I_STORE_BRIDGE_HPP

#ifdef __cplusplus

#include <string>

#include <ee/core/IPlugin.hpp>

#include "ee/store/StoreFwd.hpp"

namespace ee {
namespace store {
class IBridge : public IPlugin {
public: /// Initializes store plugin.
    [[nodiscard]] virtual Task<bool>
    initialize(const ConfigurationBuilder& builder,
               const std::shared_ptr<ITransactionLog>& transactionLog,
               const std::shared_ptr<ILibraryAnalytics>& analytics) = 0;

    /// Gets all registered products.
    virtual std::shared_ptr<ProductCollection> getProducts() const = 0;

    /// Gets subscription for the specified item.
    virtual std::shared_ptr<SubscriptionInfo>
    getSubscriptionInfo(const std::string& itemId) = 0;

    /// Asynchronously purchases an item.
    [[nodiscard]] virtual Task<bool> purchase(const std::string& itemId) = 0;

    /// Asynchronously restore transactions.
    [[nodiscard]] virtual Task<bool> restoreTransactions() = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif // EE_X_I_STORE_BRIDGE_HPP