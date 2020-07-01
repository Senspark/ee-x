//
//  PurchasingFactory.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_PURCHASING_FACTORY_HPP
#define EE_X_STORE_PURCHASING_FACTORY_HPP

#include "ee/store/StoreIExtensionProvider.hpp"
#include "ee/store/StoreIPurchasingBinder.hpp"

namespace ee {
namespace store {
class PurchasingFactory : public IPurchasingBinder, public IExtensionProvider {
public:
    explicit PurchasingFactory(
        const std::shared_ptr<IPurchasingModule>& first,
        const std::vector<std::shared_ptr<IPurchasingModule>>&
            remainingModules);

    const std::string& storeName() const;
    const std::shared_ptr<IStore>& service() const;
    void service(const std::shared_ptr<IStore>& store);

    virtual void registerStore(const std::string& name,
                               const std::shared_ptr<IStore>& store) override;
    virtual void registerExtension(
        const std::shared_ptr<IStoreExtension>& instance) override;
    virtual void registerConfiguration(
        const std::shared_ptr<IStoreController>& instance) override;

private:
    std::shared_ptr<IStore> store_;
    std::string storeName_;
}; // namespace store
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASING_FACTORY_HPP */
