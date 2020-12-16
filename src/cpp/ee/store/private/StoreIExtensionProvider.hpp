//
//  StoreIExtensionProvider.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_I_EXTENSION_PROVIDER_HPP
#define EE_X_STORE_I_EXTENSION_PROVIDER_HPP

#include "ee/store/private/StoreTypeCache.hpp"

namespace ee {
namespace store {
class IExtensionProvider {
public:
    virtual ~IExtensionProvider() = default;

    template <class T>
    std::shared_ptr<T> getExtension() const {
        auto key = TypeCache::toString<T>();
        return std::dynamic_pointer_cast<T>(getExtension(key));
    }

protected:
    virtual std::shared_ptr<IStoreExtension>
    getExtension(const std::string& key) const = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_EXTENSION_PROVIDER_HPP */
