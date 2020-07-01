//
//  StoreIPurchasingBinder.hpp
//  Pods
//
//  Created by eps on 7/1/20.
//

#ifndef EE_X_STORE_I_PURCHASING_BINDER_HPP
#define EE_X_STORE_I_PURCHASING_BINDER_HPP

#ifdef __cplusplus

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class IPurchasingBinder {
public:
    virtual ~IPurchasingBinder() = default;
    virtual void registerStore(const std::string& name,
                               const std::shared_ptr<IStore>& store) = 0;
    virtual void
    registerExtension(const std::shared_ptr<IStoreExtension>& instance) = 0;
    virtual void registerConfiguration(
        const std::shared_ptr<IStoreController>& instance) = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_PURCHASING_BINDER_HPP */
