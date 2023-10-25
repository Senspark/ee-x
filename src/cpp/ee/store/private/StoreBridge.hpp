//
//  Store.hpp
//  ee_x
//
//  Created by Zinge on 5/15/17.
//
//

#ifndef EE_X_STORE_BRIDGE_HPP
#define EE_X_STORE_BRIDGE_HPP

#include "ee/store/IStoreBridge.hpp"

namespace ee {
namespace store {
class Bridge final : public IBridge {
public:
    using Destroyer = std::function<void()>;

    explicit Bridge(IMessageBridge& bridge, ILogger& logger,
                    const Destroyer& destroyer);
    virtual ~Bridge() override;

    virtual void destroy() override;

    virtual Task<bool>
    initialize(const ConfigurationBuilder& builder,
               const std::shared_ptr<ITransactionLog>& transactionLog,
               const std::shared_ptr<ILibraryAnalytics>& analytics) override;
    virtual std::shared_ptr<ProductCollection> getProducts() const override;
    virtual std::shared_ptr<SubscriptionInfo>
    getSubscriptionInfo(const std::string& itemId) override;
    virtual Task<bool> purchase(const std::string& itemId) override;
    virtual Task<bool> restoreTransactions() override;

private:
    class Listener;

    void initializeListener();

    IMessageBridge& bridge_;
    ILogger& logger_;
    Destroyer destroyer_;

    std::shared_ptr<Listener> listener_;
    std::shared_ptr<IStoreController> controller_;
    std::shared_ptr<IExtensionProvider> extensions_;
    std::shared_ptr<ILibraryAnalytics> analytics_;

    bool initialized_;
    std::unique_ptr<LambdaAwaiter<bool>> initializationAwaiter_;
    std::function<void(bool successful)> initializationResolver_;

    std::unique_ptr<LambdaAwaiter<bool>> purchaseAwaiter_;
    std::function<void(bool successful)> purchaseResolver_;

    std::unique_ptr<LambdaAwaiter<bool>> restoreTransactionsAwaiter_;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_BRIDGE_HPP */
