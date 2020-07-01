//
//  StoreIExtensionProvider.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_I_EXTENSION_PROVIDER_HPP
#define EE_X_STORE_I_EXTENSION_PROVIDER_HPP

#ifdef __cplusplus

#include <memory>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IExtensionProvider {
public:
    virtual ~IExtensionProvider() = default;

    template <class T>
    std::shared_ptr<T> getExtension();
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_EXTENSION_PROVIDER_HPP */
