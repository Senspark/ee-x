//
//  StoreIPurchasingBinder.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_I_PURCHASING_BINDER_HPP
#define EE_X_STORE_I_PURCHASING_BINDER_HPP

#include "ee/store/private/StoreTypeCache.hpp"

namespace ee {
namespace store {
class IPurchasingBinder {
public:
    virtual ~IPurchasingBinder() = default;
    virtual void registerStore(const std::string& name,
                               const std::shared_ptr<IStore>& store) = 0;

    template <class T>
    void registerExtension(const std::shared_ptr<IStoreExtension>& instance) {
        auto key = TypeCache::toString<T>();
        registerExtension(key, instance);
    }

    template <class T>
    void registerConfiguration(
        const std::shared_ptr<IStoreConfiguration>& instance) {
        auto key = TypeCache::toString<T>();
        registerConfiguration(key, instance);
    }

protected:
    virtual void
    registerExtension(const std::string& key,
                      const std::shared_ptr<IStoreExtension>& instance) = 0;

    virtual void registerConfiguration(
        const std::string& key,
        const std::shared_ptr<IStoreConfiguration>& instance) = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_PURCHASING_BINDER_HPP */
