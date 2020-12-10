//
//  StoreINativeStoreProvider.hpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#ifndef EE_X_STORE_I_NATIVE_STORE_PROVIDER_HPP
#define EE_X_STORE_I_NATIVE_STORE_PROVIDER_HPP

#include <memory>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class INativeStoreProvider {
public:
    virtual std::shared_ptr<INativeStore>
    getAndroidStore(const std::shared_ptr<IUnityCallback>& callback,
                    AppStore store,
                    const std::shared_ptr<IPurchasingBinder>& binder) const = 0;

    virtual std::shared_ptr<INativeAppleStore>
    getStoreKit(const std::shared_ptr<IUnityCallback>& callback) const = 0;
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_I_NATIVE_STORE_PROVIDER_HPP */
