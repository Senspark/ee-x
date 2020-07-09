//
//  Store.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_BRIDGE_HPP
#define EE_X_STORE_BRIDGE_HPP

#ifdef __cplusplus

#include <optional>
#include <string>
#include <vector>

#include <ee/core/IPlugin.hpp>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class Bridge final : public IPlugin {
public:
    Bridge();
    ~Bridge();

    explicit Bridge(const Logger& logger);

    virtual void destroy() override;

    /// Initializes store plugin.
    [[nodiscard]] Task<bool>
    initialize(const ConfigurationBuilder& builder,
               const std::shared_ptr<ITransactionLog>& transactionLog);

    /// Gets all registered products.
    std::shared_ptr<ProductCollection> getProducts() const;

    /// Gets subscription for the specified item.
    std::shared_ptr<SubscriptionInfo>
    getSubscriptionInfo(const std::string& itemId);

    /// Asynchronously purchases an item.
    [[nodiscard]] Task<bool> purchase(const std::string& itemId);

    /// Asynchronously restore transactions.
    [[nodiscard]] Task<bool> restoreTransactions();

private:
    class Listener;

    void initializeListener();

    IMessageBridge& bridge_;
    const Logger& logger_;

    std::shared_ptr<Listener> listener_;
    std::shared_ptr<IStoreController> controller_;
    std::shared_ptr<IExtensionProvider> extensions_;

    bool initialized_;
    std::unique_ptr<LambdaAwaiter<bool>> initializationAwaiter_;
    std::function<void(bool successful)> initializationResolver_;

    std::unique_ptr<LambdaAwaiter<bool>> purchaseAwaiter_;
    std::function<void(bool successful)> purchaseResolver_;

    std::unique_ptr<LambdaAwaiter<bool>> restoreTransactionsAwaiter_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_BRIDGE_HPP */
