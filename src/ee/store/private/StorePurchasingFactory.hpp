//
//  PurchasingFactory.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_PURCHASING_FACTORY_HPP
#define EE_X_STORE_PURCHASING_FACTORY_HPP

#include <map>

#include "ee/store/private/StoreIExtensionProvider.hpp"
#include "ee/store/private/StoreIPurchasingBinder.hpp"

namespace ee {
namespace store {
class PurchasingFactory
    : public std::enable_shared_from_this<PurchasingFactory>,
      public IPurchasingBinder,
      public IExtensionProvider {
private:
    using Self = PurchasingFactory;

public:
    static std::shared_ptr<Self>
    create(const std::shared_ptr<IPurchasingModule>& first,
           const std::vector<std::shared_ptr<IPurchasingModule>>&
               remainingModules);

    const std::string& storeName() const;
    const std::shared_ptr<IStore>& service() const;
    void service(const std::shared_ptr<IStore>& store);

    virtual void registerStore(const std::string& name,
                               const std::shared_ptr<IStore>& store) override;

    template <class T>
    std::shared_ptr<T> getConfig() const {
        auto key = TypeCache::toString<T>();
        return std::dynamic_pointer_cast<T>(getConfig(key));
    }

protected:
    virtual void registerExtension(
        const std::string& key,
        const std::shared_ptr<IStoreExtension>& instance) override;
    virtual void registerConfiguration(
        const std::string& key,
        const std::shared_ptr<IStoreConfiguration>& instance) override;
    virtual std::shared_ptr<IStoreExtension>
    getExtension(const std::string& key) const override;
    std::shared_ptr<IStoreConfiguration>
    getConfig(const std::string& key) const;

private:
    PurchasingFactory() = default;

    std::map<std::string, std::shared_ptr<IStoreConfiguration>> configMap_;
    std::map<std::string, std::shared_ptr<IStoreExtension>> extensionMap_;
    std::shared_ptr<IStore> store_;
    std::string storeName_;
}; // namespace store
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_PURCHASING_FACTORY_HPP */
